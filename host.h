/**
 * ECE408 
 * Host.h
 * Purpose: Hosts represent access points.
 * They send Packets from Flows through Links to another Host.
 * 
 * @author Jaeryung Song, Kangqiao Lei, Eric Nguyen
 * @version 0.2.0 04/19/16
 */

#ifndef HOST_H
#define HOST_H

#include <vector>
#include <string>
#include "Node.h"
#include "util.h"

class Link;
class Flow;
class Packet;

class Host : public Node {
	 private: 
		std::vector<Flow *> Flows; 

	 public: 
		// The constructor creates the Host, initially unconnected and without Flows.
		Host(std::string name): Node(name){};
		
		// Add a Flow for the Host
		int addFlow(Flow *f);
		
		// Sends a Packet into the network via the Host's Link
		// Called by the Flow and the TLA
		//virtual bool send_Pak(Packet);
		
		// Receives incoming Packet
		virtual int receive_Packet(Packet *p);
		
		// Debug
		std::string print();
};
#endif
