/**
 * ECE408 
 * Host.cpp
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei
 * @version 0.5.0 05/03/16
 */

#include <assert.h>
#include <iostream>
#include <algorithm>

#include "Host.h"
#include "Link.h"
#include "Flow.h"
#include "Packet.h"

// Add Flow to the Host
void Host::addFlow(Flow* f){
	Flows.push_back(f);
}

// Upon receive_Packet event, process and send to associated Flow
void Host::receive_pak(Packet* p, Link *l){
	// Add check if Packet is routing, drop
	std::vector<Flow *>::iterator itr = std::find(Flows.begin(), Flows.end(), p->getFlow());
	assert(!(itr == Flows.end()));
	(*itr)->receive_Pak(p);
	return;
}

// Debug print Host name and address
void Host::print() {
#ifndef NDEBUG
if (debug) {
	*debugSS<<"Host "<<getName()<<":"<<std::endl<<"Contains Flows ";
	for(auto const& value: getFlows()) {
		*debugSS<<value->getName()<<" ";
	}
		*debugSS<<std::endl<<"Connected to Links ";
	for(auto const& value: getLinks()) {
		*debugSS<<value->getName()<<" ";
	}
	*debugSS<<std::endl;
}
#endif
}