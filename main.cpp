// Hello World example
// This example shows basic usage of DOM-style API.

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
	/*
	{
        const Value& a = root["hosts"]; // Using a reference for consecutive access is handy and faster.
        assert(a.IsArray());
        for (SizeType i = 0; i < a.Size(); i++) // rapidjson uses SizeType instead of size_t.
            printf("a[%d] = %d\n", i, a[i].GetInt());
        
        int y = a[0].GetInt();
        (void)y;

        // Iterating array with iterators
        printf("a = ");
        for (Value::ConstValueIterator itr = a.Begin(); itr != a.End(); ++itr)
            printf("%d ", itr->GetInt());
        printf("\n");
    }
	*/
	
	
	/*
    // Since version 0.2, you can use single lookup to check the existing of member and its value:
	Value::MemberIterator hello = root.FindMember("hosts");
    assert(hello != root.MemberEnd());
    assert(hello->value.IsString());
    assert(strcmp("world", hello->value.GetString()) == 0);
    (void)hello;

    assert(root["t"].IsBool());     // JSON true/false are bool. Can also uses more specific function IsTrue().
    printf("t = %s\n", root["t"].GetBool() ? "true" : "false");

    assert(root["f"].IsBool());
    printf("f = %s\n", root["f"].GetBool() ? "true" : "false");

    printf("n = %s\n", root["n"].IsNull() ? "null" : "?");

    assert(root["i"].IsNumber());   // Number is a JSON type, but C++ needs more specific type.
    assert(root["i"].IsInt());      // In this case, IsUint()/IsInt64()/IsUInt64() also return true.
    printf("i = %d\n", root["i"].GetInt()); // Alternative (int)root["i"]

    assert(root["pi"].IsNumber());
    assert(root["pi"].IsDouble());
    printf("pi = %g\n", root["pi"].GetDouble());




    ////////////////////////////////////////////////////////////////////////////
    // 3. Modify values in root.

    // Change i to a bigger number
    {
        uint64_t f20 = 1;   // compute factorial of 20
        for (uint64_t j = 1; j <= 20; j++)
            f20 *= j;
        root["i"] = f20;    // Alternate form: root["i"].SetUint64(f20)
        assert(!root["i"].IsInt()); // No longer can be cast as int or uint.
    }

    // Adding values to array.
    {
        Value& a = root["a"];   // This time we uses non-const reference.
        Document::AllocatorType& allocator = root.GetAllocator();
        for (int i = 5; i <= 10; i++)
            a.PushBack(i, allocator);   // May look a bit strange, allocator is needed for potentially realloc. We normally uses the root's.

        // Fluent API
        a.PushBack("Lua", allocator).PushBack("Mio", allocator);
    }

    // Making string values.

    // This version of SetString() just store the pointer to the string.
    // So it is for literal and string that exists within value's life-cycle.
    {
        root["hello"] = "rapidjson";    // This will invoke strlen()
        // Faster version:
        // root["hello"].SetString("rapidjson", 9);
    }

    // This version of SetString() needs an allocator, which means it will allocate a new buffer and copy the the string into the buffer.
    Value author;
    {
        char buffer2[10];
        int len = sprintf(buffer2, "%s %s", "Milo", "Yip");  // synthetic example of dynamically created string.

        author.SetString(buffer2, static_cast<SizeType>(len), root.GetAllocator());
        // Shorter but slower version:
        // root["hello"].SetString(buffer, root.GetAllocator());

        // Constructor version: 
        // Value author(buffer, len, root.GetAllocator());
        // Value author(buffer, root.GetAllocator());
        memset(buffer2, 0, sizeof(buffer2)); // For demonstration purpose.
    }
    // Variable 'buffer' is unusable now but 'author' has already made a copy.
    root.AddMember("author", author, root.GetAllocator());

    assert(author.IsNull());        // Move semantic for assignment. After this variable is assigned as a member, the variable becomes null.

    ////////////////////////////////////////////////////////////////////////////
    // 4. Stringify JSON

    //printf("\nModified JSON with reformatting:\n");
    //StringBuffer sb;
    //PrettyWriter<StringBuffer> writer(sb);
    //root.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
    //puts(sb.GetString());
	*/
    return 0;
}
