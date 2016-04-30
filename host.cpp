/**
 * ECE408 
 * Host.cpp
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei
 * @version 0.2.0 04/19/16
 */

#include <assert.h>
#include <iostream>
#include <algorithm>
#include "Host.h"
#include "Link.h"
#include "Flow.h" // Included because we call one of Flow's functions
#include "Packet.h"

// Add Flow to the Host
void Host::addFlow(Flow* f){
	Flows.push_back(f);
}

// Upon receive_Packet event, process and send to associated Flow
void Host::receive_pak(Packet* p){
	// Add check if Packet is routing, drop
	std::vector<Flow *>::iterator itr = std::find(Flows.begin(), Flows.end(), p->getFlow());
	assert(!(itr == Flows.end()));
	(*itr)->receive_Pak(p);
	return;
}

// Gets the other Node connected to the Link
Node* Host::getConnectedNode(Link *connection){
	return connection->getOtherNode(this);
}

// Debug print Host name and address
void Host::print() {
	*debugSS << "Host " << getName() << ":" << std::endl << "Contains Flows ";
		for(auto const& value: getFlows()) {
			*debugSS << value->getName() << " ";
		}
	*debugSS << std::endl << "Connected to Links ";
		for(auto const& value: getLinks()) {
			*debugSS << value->getName() << " ";
		}
	*debugSS << std::endl;
}
