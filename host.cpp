/**
 * ECE408 
 * Host.cpp
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei
 * @version 0.2.0 04/19/16
 */
 
#include <iostream>
#include <algorithm>
#include "Host.h"
//#include "Link.h"
#include "Flow.h" // Included because we call one of Flow's functions

// Initialize and Link the Host to the network as an endpoint
void Host::addLink(Link* l){
	// Hosts should only have one connection
	assert(Links.empty());
	Links.push_back(l);
}

// Add Flow to the Host
void Host::addFlow(Flow* f){
	Flows.push_back(f);
}

// Upon receive_Packet event, process and send to associated Flow
int Host::receive_pak(Packet* p){
	// Add check if Packet is routing, drop
	std::vector<Flow *>::iterator itr = std::find(Flows.begin(), Flows.end(), p->getFlow());
	assert(!itr == Flows.end());
	(*itr)->receive_pak(p);
	return 0;
}

// Gets the other Node connected to the Link
Node* getConnectedNode(Link *connection){
	return connection->getOtherNode(&this);
}

// Debug print Host name and address

