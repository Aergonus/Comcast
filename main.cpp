/**
 * ECE408 
 * comcast.cpp
 * Purpose: Network Simulator Entry Point
 * 
 * @author Kangqiao Lei
 * @version 0.1.5 04/05/16
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

//#include <fstream>

//#include "rapidjson/prettywriter.h" // for stringify JSON

using namespace std;

bool debug = false;
ostream &debugSS = cout;
ostream &errorSS = cerr;

int parseInputs(net &Network, string inputFile) {
	using namespace rapidjson;
	Document root; // root is a JSON value represents the root of DOM.
#ifndef NDEBUG
    printf("Parse a JSON file to document root.\n");
#endif
	
	FILE *input = fopen(inputFile.c_str(), "rb"); // "r" for non-Windows
	if (input!=NULL) {
		char readBuffer[65536];
		rapidjson::FileReadStream json(input, readBuffer, sizeof(readBuffer));
		printf("Original JSON:\n %s\n", json);
		if (root.ParseStream(json).HasParseError()) {
			fprintf(stderr, "\nError(offset %u): %s\n", 
				(unsigned)root.GetErrorOffset(),
				GetParseError_En(root.GetParseError()));
			return 1;
		}
	} else {
		cout << "Unable to open file " << inputFile << endl; 
		return -1;
	}
#ifndef NDEBUG
    printf("Parsing to root succeeded.\n");
#endif

    assert(root.IsObject());    // Root can be either an object or array. In our template we defined it as an object
	
	Value::MemberIterator end = root.FindMember("end"); // assert(root.HasMember("hosts")); // Old version
	float endtime = (end != root.MemberEnd()) ? end.GetDouble() : 0;
	assert(endtime >= 0);
	Network.setEnd(endtime);
#ifndef NDEBUG
    printf("Set end time of simulator: %f.\n", endtime);
#endif	
	
    {
		assert(root.HasMember("hosts"));
        const Value& hosts = root["hosts"]; 
        assert(hosts.IsArray());

        for (Value::ConstValueIterator itr = hosts.Begin(); itr != hosts.End(); ++itr) {
			Network.addHost(itr->GetString());
#ifndef NDEBUG
			printf("Added Host %s\n", itr->GetString());
#endif
		}
    }
#ifndef NDEBUG
    printf("Finished Adding Hosts.\n");
#endif	

    {
		assert(root.HasMember("routers"));
        const Value& routers = root["routers"]; 
        assert(routers.IsArray());

        for (Value::ConstValueIterator itr = routers.Begin(); itr != routers.End(); ++itr) {
			Network.addRouter(itr->GetString());
#ifndef NDEBUG
			printf("Added Router %s\n", itr->GetString());
#endif
		}
    }
#ifndef NDEBUG
    printf("Finished Adding Routers.\n");
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
			printf("Added Link %s\n", clink.[id].GetString());
#endif
		}
    }
#ifndef NDEBUG
    printf("Finished Adding Links.\n");
#endif	


    {
		assert(root.HasMember("flows"));
        const Value& flows = root["flows"]; 
        assert(flows.IsArray());

		for (SizeType i = 0; i < flows.Size(); ++i) {
			assert(flows[i].IsObject());
			const Value& cflow = flows[i];
			Network.addLink(cflow.[id].GetString(), cflow.[node_src].GetString(), cflow[node_dst].GetString(), ...
				(float) cflow[data_size].GetDouble(), (float) cflow[start_time].GetDouble());
#ifndef NDEBUG
			printf("Added Flow %s\n", cflow.[id].GetString());
#endif
		}
    }
#ifndef NDEBUG
    printf("Finished Adding Flows.\n");
#endif
	
	return 0;
}

int main(int argc, char *argv[]) {
	int c = -1, b = 0; // getopt options
	static char usageInfo[] = "[-i input_file] [-o output_file] [-d]\n"; // Prompt on invalid input
	string inputFile, outputFile;
	
#ifndef NDEBUG
    printf("Parsing options if they exist.\n");
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
                fprintf(stderr, "Error Invalid option: %c\n", c);
                return -1;
				break;
            default:
                fprintf(stderr, "Usage: %s %s", argv[0], usageInfo);
		}
	}
	if (inputFile.empty()) {
		cout << "Please specify the network topology input file:\n>";
		getline(cin, inputFile);
	}

	// Create Network Simulator object 
	net Network();
#ifndef NDEBUG
    printf("Created Network Simulator object.\n");
#endif	
	
	// Load JSON Input File
	parseInputs(&Network, inputFile);
#ifndef NDEBUG
    printf("Loaded Network Topology.\n");
#endif
	
	Network.run();

    return 0;
}