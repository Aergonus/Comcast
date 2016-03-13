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

#include <link.h>

class node (
	private:
		std::string name;
		std::vector<link *> links; 
		// bool routerorhostnode?
	public: 
		node(const std::string name);
		node getConnectedNode(link &connection);
		int addLink(link &plugInLink);
		vector<link *> getLinks() {return links;};
		void print();
);

#endif