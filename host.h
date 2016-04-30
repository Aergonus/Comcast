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
#include "node.h"
#include "util.h"

class Link;
class Flow;
class Packet;

class Host : public Node {
	 private: 
		std::vector<Flow *> Flows; 

	 public: 
		// The constructor creates the Host, initially unconnected and without Flows.
		Host(std::string name): Node(name){
			isRouter = false;
		};
		~Host(){};
		// Add a Flow for the Host
		void addFlow(Flow *f);
		
		// Add a Link for the Host
		void addLink(Link *l);
		
		// Sends a Packet into the network via the Host's Link
		// Called by the Flow and the TLA
		//virtual bool send_Pak(Packet);
		
		Node* getConnectedNode(Link *);
		
		// Receives incoming Packet
		void receive_pak(Packet *p);
		
		// Debug
		std::string print();
		
		bool operator== (Host *cmpHost){
			return (this->getName() == cmpHost->getName());
		};
};
#endif
