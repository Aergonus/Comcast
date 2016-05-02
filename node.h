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
		virtual ~Node(){};
		bool isRouter;
		std::string getName(){return name;} // Gets address of the Node
		void addLink(Link *l){Links.push_back(l);}; // Add Link to the Node
		std::vector<Link *> getLinks(){return Links;}; // Gets list of Links connected to the Node
		virtual void receive_pak(Packet *p) = 0;
		virtual Node* getConnectedNode(Link *connection) = 0; // Gets the Link connected to the given Node

		bool operator == (Node *cmpNode){
			return (this->getName() == cmpNode->getName());
		};

		// Debug
		virtual void print() = 0;
};

#endif
