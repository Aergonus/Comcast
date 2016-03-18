#ifndef HOST_H
#define HOST_H

#include <string>
#include <vector>

#include "flow.h"
#include "node.h"
#include "util.h"

class link;
class packet;
class host : public node {
	 private: 
		std::vector<link*> links;
		std::vector<flow *> flows;
		int address; // TODO: size? make str? IPv4 is 32 bits IPv6 is 128 bits
	 public: 
	 	//CONSTRUCTORS
		host(std::string name,int address):node(name),address(address){}
		//FUNCTIONS
		//add link pointer in link vector
		void addLink(link* l);
		//add flow pointer in flows vector
		void addFlow(flow* f);
		//sends a packet into the network via the host's link
		virtual bool sendPak();
		//receives packet from a link
		virtual int receive_packet(packet* p);
 };
#endif