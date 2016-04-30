/**
 * ECE408 
 * comcast.cpp
 * Purpose: Network Simulator Entry Point
 * 
 * @author Kangqiao Lei
 * @version 0.2.0 04/21/16
 */

//#define NDEBUG // Comment out to turn on debug information and assertions

#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

#include "rapidjson/document.h"	 // rapidjson's DOM-style API
#include "rapidjson/error/en.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "net.h"

//#include "rapidjson/prettywriter.h" // for stringify JSON

bool debug = false;
std::ostream *debugSS = &std::cout;
std::ostream *errorSS = &std::cerr;
std::ostream *outputSS = &std::cout;

int parseInputs(net &Network, std::string inputFile) {
	rapidjson::Document root; // root is a JSON value represents the root of DOM.
#ifndef NDEBUG
	*debugSS << "Parse a JSON file to document root." << std::endl;
#endif
	
	FILE *input = fopen(inputFile.c_str(), "rb"); // "r" for non-Windows
	if (input!=NULL) {
		char readBuffer[65536];
		rapidjson::FileReadStream json(input, readBuffer, sizeof(readBuffer));
		//*debugSS << "Original JSON:\n" << json << endl;
		if (root.ParseStream(json).HasParseError()) {
			*errorSS << "Something fucked up with parsing Root." << std::endl;
			/*
			fprintf(*errorSS, "\nError(offset %u): %s\n", 
				(unsigned)root.GetErrorOffset(),
				GetParseError_En(root.GetParseError()));
			*/
			return 1;
		}
		fclose(input);
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		root.Accept(writer);
		*debugSS << "Original JSON:\n" << buffer.GetString() << std::endl;
	} else {
		*errorSS << "Unable to open file " << inputFile << std::endl; 
		return -1;
	}
#ifndef NDEBUG
	*debugSS<< "Parsing to root succeeded." << std::endl;
#endif

	assert(root.IsObject());	// Root can be either an object or array. In our template we defined it as an object
	
	rapidjson::Value::MemberIterator end = root.FindMember("end"); // assert(root.HasMember("Hosts")); // Old version
	float endtime = (end != root.MemberEnd()) ? end->value.GetDouble() : 0;
	assert(endtime >= 0);
	if (endtime > 0) {
		Network.setEnd(endtime);
	}
#ifndef NDEBUG
	*debugSS << "Set end time of simulator: " << endtime << std::endl;
#endif	
	
	{
		assert(root.HasMember("Hosts"));
		const rapidjson::Value& Hosts = root["Hosts"]; 
		assert(Hosts.IsArray());

		for (rapidjson::Value::ConstValueIterator itr = Hosts.Begin(); itr != Hosts.End(); ++itr) {
			Network.addHost(itr->GetString());
#ifndef NDEBUG
		*debugSS << "Attempted to add Host " << itr->GetString() << std::endl;;
#endif
		}
	}
#ifndef NDEBUG
	*debugSS << "Finished Adding Hosts." << std::endl;
#endif	

	{
		// Test Case 0 has no Routers
		//assert(root.HasMember("Routers"));
		const rapidjson::Value& Routers = root["Routers"]; 
		//assert(Routers.IsArray());

		for (rapidjson::Value::ConstValueIterator itr = Routers.Begin(); itr != Routers.End(); ++itr) {
			Network.addRouter(itr->GetString());
#ifndef NDEBUG
		*debugSS << "Attempted to add Router " << itr->GetString() << std::endl;
#endif
		}
	}
#ifndef NDEBUG
	*debugSS << "Finished Adding Routers." << std::endl;
#endif	


	{
		assert(root.HasMember("Links"));
		const rapidjson::Value& Links = root["Links"]; 
		assert(Links.IsArray());
		for (rapidjson::SizeType i = 0; i < Links.Size(); ++i) {
			assert(Links[i].IsObject());
			const rapidjson::Value& cLink = Links[i];
			const rapidjson::Value& endpoints = cLink["endpoints"];
			Network.addLink(cLink["id"].GetString(), endpoints[0].GetString(), endpoints[1].GetString(), 
				(float) cLink["rate"].GetDouble() * BYTES_PER_MEGABIT, (float) cLink["delay"].GetDouble() / MS_PER_SEC , (float) cLink["buffer"].GetDouble() * BYTES_PER_KB);
#ifndef NDEBUG
			*debugSS <<"Attempted to add Link " << cLink["id"].GetString() << std::endl;
#endif
		}
	}
#ifndef NDEBUG
	*debugSS << "Finished Adding Links." << std::endl;
#endif	

	{
		assert(root.HasMember("Flows"));
		const rapidjson::Value& Flows = root["Flows"]; 
		assert(Flows.IsArray());
		TCP_type tcp_enum;
		for (rapidjson::SizeType i = 0; i < Flows.Size(); ++i) {
			assert(Flows[i].IsObject());
			const rapidjson::Value& cFlow = Flows[i];
			if (cFlow.HasMember("protocol")) {
					std::string tcp_string = cFlow["protocol"].GetString();
					std::transform(tcp_string.begin(), tcp_string.end(), tcp_string.begin(), ::toupper);
				if (tcp_string == "TAHOE") {
					tcp_enum = TAHOE;
				} else if (tcp_string == "RENO") {
					tcp_enum = RENO;
				}
			} else {
				tcp_enum = TAHOE;
			}
			//Network.addFlow(cFlow["id"].GetString(), cFlow["src"].GetString(), cFlow["dst"].GetString(), (float) cFlow["size"].GetDouble() * BYTES_PER_MB, (float) cFlow["start"].GetDouble(), tcp_enum);
			Network.addFlow(cFlow["id"].GetString(), cFlow["src"].GetString(), cFlow["dst"].GetString(), (float) cFlow["size"].GetDouble() * (1 << 8), (float) cFlow["start"].GetDouble(), tcp_enum);
#ifndef NDEBUG
			*debugSS << "Attempted to add Flow " << cFlow["id"].GetString() << std::endl;
#endif
		}
	}
#ifndef NDEBUG
	*debugSS << "Finished Adding Flows." << std::endl;
#endif
	
	return 0;
}

int main(int argc, char *argv[]) {
	int c = -1; // getopt options
	static char usageInfo[] = "[-i input_file] [-o output_file] [-d]\n"; // Prompt on invalid input
	std::string inputFile, outputFile, debugFile;
	std::ofstream outFile;
#ifndef NDEBUG
	*debugSS << "Parsing options if they exist." << std::endl;
#endif
	// Added : in front of arguement list to suppress errors and use custom error code
	while ((c = getopt(argc, argv, ":i:o:d")) != -1) { 
		switch (c) {
			case 'i':
				inputFile = optarg;
				break;
			case 'o':
				outputFile = optarg;
				break;
			case 'd':
				debugFile = optarg;
				debug = true;
				break;
			case ':':
				switch (optopt) {
					case 'i':
						if (inputFile.empty()) {
							std::cout << "Please specify the network topology input file:\n";
							getline(std::cin, inputFile);
						}
						break;
					case 'o':
						if (outputFile.empty()) {
							std::cout << "Please specify the output file:\n";
							getline(std::cin, outputFile);
						}
						break;
					//case 'd':
					//	cout << "No debug file specified. Defaulting to cout." << endl;
					//	break;
				}
				break;
			case '?':
				*errorSS << "Error Invalid option: " << c << std::endl;
				return -1;
				break;
			default:
				*errorSS << "Usage: " << argv[0] << " " <<  usageInfo << std::endl;
		}
	}
	if (inputFile.empty()) {
		std::cout << "Please specify the network topology input file:\n";
		getline(std::cin, inputFile);
	}
	if (outputFile.empty()) {
		std::cout << "Please specify the output file:\n";
		getline(std::cin, outputFile);
	}
	outFile.open(outputFile.c_str());
	if (outFile.fail()) {
		std::cerr << "Failed to open output file " << outputFile << ". Are you sure you want to use cout? (y/N)" << std::endl;
		getline(std::cin, outputFile);
		if (outputFile  == "y") {
			return -1;
		}
	} else {
		outputSS = &outFile;
	}
#ifndef NDEBUG
	inputFile = "./input/test_case_0.json";
	outputFile = "./output.csv";
#endif
	// Create Network Simulator object 
#ifndef NDEBUG
	*debugSS << "Created Network Simulator object." << std::endl;
#endif
	
	// Load JSON Input File
	net *Network = new net();
	parseInputs(*Network, inputFile);
#ifndef NDEBUG
	*debugSS << "Loaded Network Topology." << std::endl << std::endl << std::endl;
#endif
	
	Network->run();

	return 0;
}
