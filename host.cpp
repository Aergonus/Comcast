/**
 * ECE408 
 * host.cpp
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei
 * @version 0.1.5 04/05/16
 */
 
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "host.h"
//#include "link.h"
#include "flow.h" // Included because we call one of flow's functions

// Initialize and link the host to the network as an endpoint
void host::addLink(link* l){
	// Hosts should only have one connection
	assert(links.empty());
	links.push_back(l);
}

void host::addFlow(flow* f){
	flows.push_back(f);
}

// Upon receive_packet event, process and send to associated flow
int host::receive_pak(packet* p){
    std::vector<flow *>::iterator itr = std::find(flows.begin(), flows.end(), p->getFlow());
    assert(!itr == flows.end());
	(*itr)->receive_pak(p);
	return 0;
}

node* getConnectedNode(link *connection){
	return connection->getOtherNode(&this);
}

// Debug print host name and address