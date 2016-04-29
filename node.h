/**
 * ECE408 
 * Node.h
 * Purpose: Nodes are points of the network, i.e. Hosts and Routers
 * 
 * @author Jaeryung Song, Kangqiao Lei
 * @version 0.2.0 04/19/16
 */

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "util.h"

class Link;
class Packet;

class Node {
	private:
		std::string name; //Node id
		std::vector<Link *> Links; // list of Links connected to Node

	public: 
		// Constructor
		Node(const std::string name):name(name){};
		
		std::string getName(){return name;} // Gets address of the Node
		int addLink(Link *l); // Add Link to the Node
		std::vector<Link *> getLinks(){return Links;}; // Gets list of Links connected to the Node
		virtual int receive_Packet(Packet *p) = 0;
		Node* getConnectedNode(Link *connection); // Gets the Link connected to the given Node
		
		// Debug
		std::string print();
};
#endif
