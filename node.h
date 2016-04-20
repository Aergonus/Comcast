/**
 * ECE408 
 * node.h
 * Purpose: Nodes are points of the network, i.e. hosts and routers
 * 
 * @author Kangqiao Lei
 * @version 0.2.0 04/19/16
 */

#ifndef NODE_H
#define NODE_H

#include "util.h"

class link;
class packet;

class node {
	private:
		std::string name;
		std::vector<link *> links; 

	public: 
		
		node(const std::string name):name(name){};
		
		std::string getName(){return name;}
		std::vector<link *> getLinks(){return links;};
		virtual int receive_packet(packet *p) = 0;
		node* getConnectedNode(link *connection);
		int addLink(link *l);
		
		void print();
};
#endif