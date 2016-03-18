/**
 * ECE408 
 * node.h
 * Purpose: Nodes are points of the network, i.e. hosts and routers
 * 
 * @author Kangqiao Lei
 * @version 0.1 03/13/16
 */
#ifndef NODE_H
#define NODE_H

//#include "link.h"
#include "util.h"
class link;
class packet;
class node {
	private:
		std::string name;
		std::vector<link *> links; 
		// bool routerorhostnode?
	public: 
		virtual int receive_packet(packet*) = 0;
		node(const std::string name): name(name){};
		node* getConnectedNode(link &connection);
		int addLink(link &plugInLink);
		std::vector<link *> getLinks();
		void print();
};

#endif