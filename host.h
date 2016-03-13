/**
 * ECE408 
 * host.h
 * Purpose: Hosts represent access points.
 * They send packets from flows through Links to another Host.
 * 
 * @author Kangqiao Lei
 * @version 0.1 03/13/16
 */

#ifndef HOST_H
#define HOST_H

#include <vector>
#include <string>
#include <flow.h>
#include "node.h" // node should include links

 // TODO: Derive from Node and move whatever is needed to node.
 class host : public node {
	 private: 
		// Each host has an associated name and IP address. 
		std::string name;
		std::vector<flow *> flows; 
		std::vector<link *> links; 
		int address; // TODO: size? make str? IPv4 is 32 bits IPv6 is 128 bits
		
	 public: 
		// The constructor creates the host,
		// which is initially unconnected and without flows.
		host(name, address); // TODO: remove address and autoassign?
		
		// Initialize and link the host to the network as an endpoint
		int addLink(link);
		// Create a link to a Router
		int linkRouter(Router);
		// Add a flow for the Host
		int addFlow(flow);
		// Sends a packet into the network via the host's link
		// Called by the Flow and the TLA
		int sendPak(packet);
		
		// Upon receive_packet event, process and send to associated flow
		int recieve_packet(event);
		
		// Debug print host name and address
		void print(); 
 };
 
#endif