/**
 * ECE408 
 * host.h
 * Purpose: Hosts represent access points.
 * They send packets from flows through Links to another Host.
 * 
 * @author  Eui Han
 * @version 0.2.0 04/19/16
 */

#ifndef ROUTER_H
#define ROUTER_H

#include <vector>
#include <string>
#include "node.h"

class Link;
class flow;
class packet;

class router : public node {
	 private: 
		std::vector<flow *> flows; 
                Link* l;
	 public: 
		// The constructor creates the host, initially unconnected and without flows.
		router(std::string name): node(name){};
		
		// Add a flow for the Host
		int addFlow(flow *f);
		Link* getLink();
		// Sends a packet into the network via the host's link
		// Called by the Flow and the TLA
		//virtual bool send_Pak(packet);
		
		// Receives incoming packet
		virtual int receive_packet(packet *p);
                Link* getLink();
                node* getConnectedNode(Link*);
};
#endif
