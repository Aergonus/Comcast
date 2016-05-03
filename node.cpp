/**
 * ECE408 
 * Node.cpp
 * Purpose: Nodes are points of the network, i.e. Hosts and Routers
 * 
 * @author Jaeryung Song, Kangqiao Lei
 * @version 0.5.0 05/03/16
 */

#include "Node.h"
#include "Link.h"

void Node::addNeighbor(Link *l, Node *n){
	neighbors.insert({l->getName(),n});
	addLink(l);
}

Node* Node::getConnectedNode(Link *connection){
	return connection->getOtherNode(this);
}

Link* Node::getLink(std::string lid){
	std::vector<Link *>::iterator itr = Links.begin();
	while(itr != Links.end()) {
		if ((*itr)->getName() == lid) {
			return *itr;
		}
		itr++;
	}
	return NULL;
}