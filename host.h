/**
 * ECE408 
 * host.h
 * Purpose: Hosts represent access points.
 * They send packets from flows through Links to another Host.
 * 
 * @author Jaeryung Song, Kangqiao Lei, Eric Nguyen
 * @version 0.2.0 04/19/16
 */

#ifndef HOST_H
#define HOST_H

#include <vector>
#include <string>
#include "node.h"
#include "util.h"

class link;
class flow;
class packet;

class host : public node {
	 private: 
		std::vector<flow *> flows; 

	 public: 
		// The constructor creates the host, initially unconnected and without flows.
		host(std::string name): node(name){};
		
		// Add a flow for the Host
		int addFlow(flow *f);
		
		// Sends a packet into the network via the host's link
		// Called by the Flow and the TLA
		//virtual bool send_Pak(packet);
		
		// Receives incoming packet
		virtual int receive_packet(packet *p);
};
#endif