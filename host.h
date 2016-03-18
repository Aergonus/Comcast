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
#include "flow.h"
#include "node.h"
#include "link.h"
//#include "packet.h"
#include "util.h" // node should include links

 // TODO: Derive from Node and move whatever is needed to node.
//class link;
//class flow;
class packet;
//class node;
class host : public node {
	 private: 
		// Each host has an associated name and IP address. 
		std::vector<link*> links;
		std::vector<flow *> flows;
		int address; // TODO: size? make str? IPv4 is 32 bits IPv6 is 128 bits
	 public: 
		// The constructor creates the host,
		// which is initially unconnected and without flows.
		host(std::string name,int address):node(name),address(address){
		//	this.name = name;
		//	this.address = address;
		}
		// Initialize and link the host to the network as an endpoint
		void addLink(link* l){
			links.push_back(l);
		}
		// Create a link to a Router
		//int linkRouter(Router);
		// Add a flow for the Host
		//int addFlow(flow);
		void addFlow(flow* f){
			flows.push_back(f);
		}
		// Sends a packet into the network via the host's link
		// Called by the Flow and the TLA
		bool sendPak(){
			for(int i=0; i < flows[0]->numPack(); i++ ){
				if (! links[0]->receive_pkt(flow[0]->getPacket(i), this*)){
					return false;
				}
			}
			return true;
		}
		
		// Upon receive_packet event, process and send to associated flow
		virtual int receive_packet(packet* p){
				cout<< "packets received from "<< p->getOrigin()<<endl;
				return 1;
		}
		// Debug print host name and address
		void print(); 
 };
#endif