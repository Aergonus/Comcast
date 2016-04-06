/**
 * ECE408 
 * host.h
 * Purpose: Hosts represent access points.
 * They send packets from flows through Links to another Host.
 * 
 * @author Kangqiao Lei, Eric Nguyen
 * @version 0.1.5 04/05/16
 */

#ifndef HOST_H
#define HOST_H

#include <vector>
#include <string>
#include "flow.h"
#include "node.h"
#include "util.h"

class link;
class packet;

class host : public node {
	 private: 
		std::vector<flow *> flows; 
		std::vector<link *> links; 
		
	 public: 
		// The constructor creates the host, initially unconnected and without flows.
		host(std::string name):node(name){};
		
		// Initialize and link the host to the network as an endpoint
		int addLink(link *l);
		// Add a flow for the Host
		int addFlow(flow *f);
		
		// Sends a packet into the network via the host's link
		// Called by the Flow and the TLA
		//virtual bool send_Pak(packet);
		
		// Upon receive_packet event, process and send to associated flow
		virtual int recieve_packet(packet *p);
		
		// Debug print host name and address
		void print(); 
 };
#endif