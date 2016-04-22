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
#include <iostream>
#include <unistd.h>

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/error/en.h"
#include "rapidjson/filereadstream.h"

#include "net.h"
#include "util.h"

//#include <fstream>

//#include "rapidjson/prettywriter.h" // for stringify JSON

using namespace std;

bool debug = false;
ostream &debugSS = cout;
ostream &errorSS = cerr;
ostream &outputSS;

int parseInputs(net &Network, string inputFile) {
	using namespace rapidjson;
	Document root; // root is a JSON value represents the root of DOM.
#ifndef NDEBUG
    debugSS << "Parse a JSON file to document root." << endl;
#endif
	
	FILE *input = fopen(inputFile.c_str(), "rb"); // "r" for non-Windows
	if (input!=NULL) {
		char readBuffer[65536];
		rapidjson::FileReadStream json(input, readBuffer, sizeof(readBuffer));
		debugSS << "Original JSON:\n" << json << endl;
		if (root.ParseStream(json).HasParseError()) {
			fprintf(errorSS, "\nError(offset %u): %s\n", 
				(unsigned)root.GetErrorOffset(),
				GetParseError_En(root.GetParseError()));
			return 1;
		}
	} else {
		errorSS << "Unable to open file " << inputFile << endl; 
		return -1;
	}
#ifndef NDEBUG
    debugSS << "Parsing to root succeeded." << endl;
#endif

    assert(root.IsObject());    // Root can be either an object or array. In our template we defined it as an object
	
	Value::MemberIterator end = root.FindMember("end"); // assert(root.HasMember("hosts")); // Old version
	float endtime = (end != root.MemberEnd()) ? end.GetDouble() : 0;
	assert(endtime >= 0);
	Network.setEnd(endtime);
#ifndef NDEBUG
    debugSS << "Set end time of simulator: " << endtime << endl;
#endif	
	
    {
		assert(root.HasMember("hosts"));
        const Value& hosts = root["hosts"]; 
        assert(hosts.IsArray());

        for (Value::ConstValueIterator itr = hosts.Begin(); itr != hosts.End(); ++itr) {
			Network.addHost(itr->GetString());
#ifndef NDEBUG
			debugSS << "Added Host " << itr->GetString() << endl;;
#endif
		}
    }
#ifndef NDEBUG
    debugSS << "Finished Adding Hosts." << endl;
#endif	

    {
		assert(root.HasMember("routers"));
        const Value& routers = root["routers"]; 
        assert(routers.IsArray());

        for (Value::ConstValueIterator itr = routers.Begin(); itr != routers.End(); ++itr) {
			Network.addRouter(itr->GetString());
#ifndef NDEBUG
			debugSS << "Added Router " << itr->GetString() << endl;
#endif
		}
    }
#ifndef NDEBUG
    debugSS << "Finished Adding Routers." << endl;
#endif	


    {
		assert(root.HasMember("links"));
        const Value& links = root["links"]; 
        assert(links.IsArray());

		for (SizeType i = 0; i < links.Size(); ++i) {
			assert(links[i].IsObject());
			const Value& clink = links[i];
			Network.addLink(clink.[id].GetString(), clink.[node_id1].GetString(), clink[node_id2].GetString(), ...
				(float) clink[rate].GetDouble(), (float) clink[delay].GetDouble(), (float) clink[buffer].GetDouble());
#ifndef NDEBUG
			debugSS <<"Added Link " << clink.[id].GetString() << endl;
#endif
		}
    }
#ifndef NDEBUG
    debugSS << "Finished Adding Links." << endl;
#endif	


    {
		assert(root.HasMember("flows"));
        const Value& flows = root["flows"]; 
        assert(flows.IsArray());

		for (SizeType i = 0; i < flows.Size(); ++i) {
			assert(flows[i].IsObject());
			const Value& cflow = flows[i];
			if (cflow->HasMember("TCP")) {
				TCP_type tcp_enum;
				string tcp_string = cflow[TCP].GetString();
				transform(tcp_string.begin(), tcp_string.end(), tcp_string.begin(), toupper);
				if (tcp_string == "TAHOE") {
					tcp_enum = TAHOE;
				} else if (tcp_string == "RENO") {
					tcp_enum = RENO;
				}
			} else {
				tcp_enum = TAHOE;
			}
			Network.addFlow(cflow.[id].GetString(), cflow.[node_src].GetString(), cflow[node_dst].GetString(), ...
				(float) cflow[data_size].GetDouble(), (float) cflow[start_time].GetDouble(), tcp_enum);
#ifndef NDEBUG
			debugSS << "Added Flow " << cflow.[id].GetString() << endl;
#endif
		}
    }
#ifndef NDEBUG
    debugSS << "Finished Adding Flows." << endl;
#endif
	
	return 0;
}

int main(int argc, char *argv[]) {
	int c = -1, b = 0; // getopt options
	static char usageInfo[] = "[-i input_file] [-o output_file] [-d]\n"; // Prompt on invalid input
	string inputFile, outputFile;
	
#ifndef NDEBUG
    debugSS << "Parsing options if they exist." << endl;
#endif
	while ((c = getopt(argc, argv, "i:o:d")) != -1) {
		switch (c) {
            case 'i':
				inputFile = optarg;
                break;
			case 'o':
				outputFile = optarg;
				break;
			case 'd':
            	debug = true;
            	break;
			case '?':
                fprintf(errorSS, "Error Invalid option: %c\n", c);
                return -1;
				break;
            default:
                fprintf(errorSS, "Usage: %s %s", argv[0], usageInfo);
		}
	}
	if (inputFile.empty()) {
		cout << "Please specify the network topology input file:\n>";
		getline(cin, inputFile);
	}

	// Create Network Simulator object 
	net Network();
#ifndef NDEBUG
    debugSS << "Created Network Simulator object." << endl;
#endif	
	
	// Load JSON Input File
	parseInputs(&Network, inputFile);
#ifndef NDEBUG
    debugSS << "Loaded Network Topology." << endl;
#endif
	
	Network.run();

    return 0;
}
