/**
 * ECE408 
 * host.cpp
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei
 * @version 0.2.0 04/19/16
 */
 
#include <iostream>
#include <algorithm>
#include "router.h"
//#include "link.h"
#include "flow.h"// Included because we call one of flow's functions

// Initialize and link the host to the network as an endpoint
void router::addLink(Link* l){
	// Hosts should only have one connection
	assert(links.empty());
	links.push_back(l);
}

void router::addFlow(flow* f){
	flows.push_back(f);
}

// Upon receive_packet event, process and send to associated flow
int router::receive_packet(packet* p){
	// Add check if packet is routing, drop
    std::vector<flow *>::iterator itr = std::find(flows.begin(), flows.end(), p->getFlow());
    assert(!itr == flows.end());
	(*itr)->receive_pak(p);
	return 0;
}

Link* router::getLink(){
  return l;
}

node* router::getConnectedNode(Link *connection){
	return connection->getOtherNode(&this);
}

// Debug print host name and address
