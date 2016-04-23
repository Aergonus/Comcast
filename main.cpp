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
#include <algorithm>
#include <unistd.h>

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/error/en.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "net.h"
#include "util.h"
#include "tcp.h"

//#include <fstream>

//#include "rapidjson/prettywriter.h" // for stringify JSON

bool debug = false;
std::ostream &debugSS = std::cout;
std::ostream &errorSS = std::cerr;
std::ostream &outputSS;

int parseInputs(net &Network, std::string inputFile) {
	rapidjson::Document root; // root is a JSON value represents the root of DOM.
#ifndef NDEBUG
    debugSS << "Parse a JSON file to document root." << std::endl;
#endif
	
	FILE *input = fopen(inputFile.c_str(), "rb"); // "r" for non-Windows
	if (input!=NULL) {
		char readBuffer[65536];
		rapidjson::FileReadStream json(input, readBuffer, sizeof(readBuffer));
		root.ParseStream(json);
                fclose(input);
                rapidjson::StringBuffer buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                root.Accept(writer);
		debugSS << "Original JSON:\n" << buffer.GetString() << std::endl;
//		if (root.ParseStream(json).HasParseError()) {
//			fprintf(errorSS, "\nError(offset %u): %s\n", 
//				(unsigned)root.GetErrorOffset(),
//				GetParseError_En(root.GetParseError()));
//			return 1;
		}
	} else {
		errorSS << "Unable to open file " << inputFile << std::endl; 
		return -1;
	}
#ifndef NDEBUG
<<<<<<< HEAD
    debugSS << "Parsing to root succeeded." << endl;
=======
    debugSS "Parsing to root succeeded." << std::endl;
>>>>>>> origin/Documentation
#endif

rapidjson::Value::MemberIterator end = root.FindMember("end"); // assert(root.HasMember("hosts")); // Old version

	float endtime = (end != root.MemberEnd()) ? end->value.GetDouble() : 0;

	assert(endtime >= 0);

	Network.setEnd(endtime);

#ifndef NDEBUG

    debugSS << "Set end time of simulator: " << endtime << std::endl;

#endif	

	

    {

		assert(root.HasMember("hosts"));

        const rapidjson::Value& hosts = root["hosts"]; 

        assert(hosts.IsArray());



        for (rapidjson::Value::ConstValueIterator itr = hosts.Begin(); itr != hosts.End(); ++itr) {

			Network.addHost(itr->GetString());

#ifndef NDEBUG

			debugSS << "Added Host " << itr->GetString() << std::endl;;

#endif

		}

    }

#ifndef NDEBUG

    debugSS << "Finished Adding Hosts." << std::endl;

#endif	



    {

		assert(root.HasMember("routers"));

        const rapidjson::Value& routers = root["routers"]; 

        assert(routers.IsArray());



        for (rapidjson::Value::ConstValueIterator itr = routers.Begin(); itr != routers.End(); ++itr) {

			Network.addRouter(itr->GetString());

#ifndef NDEBUG

			debugSS << "Added Router " << itr->GetString() << std::endl;

#endif

		}

    }

#ifndef NDEBUG

    debugSS << "Finished Adding Routers." << std::endl;

#endif	





    {

		assert(root.HasMember("links"));

        const rapidjson::Value& links = root["links"]; 

        assert(links.IsArray());



		for (rapidjson::SizeType i = 0; i < links.Size(); ++i) {

			assert(links[i].IsObject());

			const rapidjson::Value& clink = links[i];

			Network.addLink(clink["id"].GetString(), clink["node_id1"].GetString(), clink["node_id2"].GetString(), (float) clink["rate"].GetDouble(), (float) clink["delay"].GetDouble(), (float) clink["buffer"].GetDouble());

#ifndef NDEBUG

			debugSS <<"Added Link " << clink["id"].GetString() << std::endl;

#endif

		}

    }

#ifndef NDEBUG

    debugSS << "Finished Adding Links." << std::endl;

#endif	





    {

		assert(root.HasMember("flows"));

        const rapidjson::Value& flows = root["flows"]; 

        assert(flows.IsArray());

        TCP_type tcp_enum;

		for (rapidjson::SizeType i = 0; i < flows.Size(); ++i) {

			assert(flows[i].IsObject());

			const rapidjson::Value& cflow = flows[i];

			if (cflow.HasMember("TCP")) {

                                std::string tcp_string = cflow["TCP"].GetString();

                                std::transform(tcp_string.begin(), tcp_string.end(), tcp_string.begin(), ::toupper);

				if (tcp_string == "TAHOE") {

					tcp_enum = TAHOE;

				} else if (tcp_string == "RENO") {

					tcp_enum = RENO;

				}

			} else {

                                 tcp_enum = TAHOE;

			}

			Network.addFlow(cflow["id"].GetString(), cflow["node_src"].GetString(), cflow["node_dst"].GetString(), (float) cflow["data_size"].GetDouble(), (float) cflow["start_time"].GetDouble(), tcp_enum);

#ifndef NDEBUG

			debugSS << "Added Flow " << cflow["id"].GetString() << std::endl;

#endif

		}

    }

#ifndef NDEBUG

    debugSS << "Finished Adding Flows." << std::endl;

#endif

	

	return 0;

}



int main(int argc, char *argv[]) {

	int c = -1, b = 0; // getopt options

	static char usageInfo[] = "[-i input_file] [-o output_file] [-d]\n"; // Prompt on invalid input

        std::string inputFile, outputFile;

	

#ifndef NDEBUG

    debugSS << "Parsing options if they exist." << std::endl;

#endif

/*	while ((c = getopt(argc, argv, "i:o:d")) != -1) {

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

*/

inputFile = "./input/test_case_0.json";

outputFile = "./output.csv";

	// Create Network Simulator object 

#ifndef NDEBUG

    debugSS << "Created Network Simulator object." << std::endl;

#endif	

	

	// Load JSON Input File

        net Network;

	parseInputs(Network, inputFile);

#ifndef NDEBUG

    debugSS << "Loaded Network Topology." << std::endl;

#endif

	

	Network.run();



    return 0;
<<<<<<< HEAD
=======

>>>>>>> origin/Documentation
}
