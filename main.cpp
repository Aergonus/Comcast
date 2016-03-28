/**
 * ECE408 
 * node.h
 * Purpose: Nodes are points of the network, i.e. hosts and routers
 * 
 * @author Kangqiao Lei
 * @version 0.1 03/28/16
 */

#include <cstdio>
#include <string>
#include <iostream>
#include <unistd.h>

//#define NDEBUG // Comment out to turn on debug information and assertions

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/error/en.h"
#include "rapidjson/filereadstream.h"


//#include <iostream>
//#include <fstream>

//#include "rapidjson/prettywriter.h" // for stringify JSON

using namespace rapidjson;
using namespace std;

int main(int argc, char *argv[]) {
	bool debug = false;
	int c = -1, b = 0; // getopt options
	static char usageInfo[] = "[-i input_file] [-o output_file] [-d]\n"; // Prompt on invalid input
	string inputFile, outputFile;
	Document root; // root is a JSON value represents the root of DOM.
	
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
	
	#ifndef NDEBUG
    printf("Access values in root:\n");
    // Iterating object members
    static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
    for (Value::ConstMemberIterator itr = root.MemberBegin(); itr != root.MemberEnd(); ++itr)
        printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
	#endif

    assert(root.IsObject());    // Root can be either an object or array. In our template we defined it as an object
    Value::MemberIterator hosts = root.FindMember("hosts"); // assert(root.HasMember("hosts")); // Old version
    assert(hosts != root.MemberEnd());
	#ifndef NDEBUG
    {
        const Value& hosts = root["hosts"]; // Using a reference for consecutive access is handy and faster.
        assert(hosts.IsArray());
        for (SizeType i = 0; i < hosts.Size(); i++) // rapidjson uses SizeType instead of size_t.
            printf("hosts[%d] = %s\n", i, hosts[i].GetString());
        
        // Iterating array with iterators
        printf("Host inputs: ");
        for (Value::ConstValueIterator itr = hosts.Begin(); itr != hosts.End(); ++itr)
            printf("%s ", itr->GetString());
        printf("\n");
    }
	#endif

    return 0;
}
