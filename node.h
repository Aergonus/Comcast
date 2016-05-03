/**
 * ECE408 
 * Node.h
 * Purpose: Nodes are points of the network, i.e. Hosts and Routers
 * 
 * @author Jaeryung Song, Kangqiao Lei
 * @version 0.5.0 05/03/16
 */

#ifndef NODE_H
#define NODE_H

#include <map>
#include <vector>
#include <string>
#include "util.h"

class Link;
class Packet;

class Node {
	private:
		std::string name; // Node id
		std::vector<Link *> Links; // List of Links connected to Node
		std::map<std::string, Node *> neighbors; // Maps id of a Link to the connected Node
		void addLink(Link *l){Links.push_back(l);}; // Add Link to the Node
		
	public: 
		// Constructor
		Node(const std::string name):name(name){};
		virtual ~Node(){};

		bool isRouter;
		std::string getName(){return name;} // Gets address of the Node
		Link* getLink(std::string lid); // Gets Link by name
		std::vector<Link *> getLinks(){return Links;}; // Gets list of Links connected to the Node
		Node* getConnectedNode(Link *connection); // Gets the other Node connected to the Link
		
		void addNeighbor(Link *l, Node *n); // Initialization
		std::map<std::string, Node *> getNeighbors(){return neighbors;}; // Gets the Neighbors connected to the Node
		
		virtual void receive_pak(Packet *p, Link *l) = 0;
		
		bool operator == (Node *cmpNode){
			return (this->getName() == cmpNode->getName());
		};

		// Debug
		virtual void print() = 0;
};

#endif