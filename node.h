/**
 * ECE408 
 * node.h
 * Purpose: Nodes are points of the network, i.e. hosts and routers
 * 
 * @author Jaeryung Song, Kangqiao Lei
 * @version 0.2.0 04/19/16
 */

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "util.h"

class link;
class packet;

class node {
	private:
		std::string name;
		std::vector<link *> links; 

	public: 
		// Constructor
		node(const std::string name):name(name){};
		
		std::string getName(){return name;} // Gets address of the node
		int addLink(link *l); // Add link to the node
		std::vector<link *> getLinks(){return links;}; // Gets list of links connected to the node
		virtual int receive_packet(packet *p) = 0;
		node* getConnectedNode(link *connection); // Gets the link connected to the given node
		
		void print();
};
#endif