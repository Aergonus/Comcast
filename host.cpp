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
#include <cassert>
#include "host.h"
#include "link.h"
#include "flow.h" // Included because we call one of flow's functions


// Initialize and link the host to the network as an endpoint
void host::addLink(Link* l){
	// Hosts should only have one connection
	assert(links.empty());
	links.push_back(l);
}

// Add flow to the host
void host::addFlow(flow* f){
	flows.push_back(f);
}

// Upon receive_packet event, process and send to associated flow
<<<<<<< HEAD
void host::receive_packet(packet* p){
	// Add check if packet is routing, drop
    std::vector<flow *>::iterator itr = std::find(flows.begin(), flows.end(), p->getFlow());
    assert(itr != flows.end());
    (*itr)->receive_packet(p);
}

// Gets the other link connected to the given node
link* host::getConnectedLink(node* connection){
  for(std::vector<link*>::iterator itr = links.begin();
      itr != links.end(); ++itr){
    if itr->hasNode(connection){
      return itr->getOtherNode(connection);
    }
    else{
      return NULL;
    }
  }
}

// Debug print host name and address

=======
int host::receive_packet(packet* p){
	// Add check if packet is routing, drop
    std::vector<flow *>::iterator itr = std::find(flows.begin(), flows.end(), p->getFlow());
    assert(!itr == flows.end());
	(*itr)->receive_packet(p);
	return 0;
}

Link* host::getLink(){
  return l;
}

node* host::getConnectedNode(Link *connection){
	return connection->getOtherNode(&this);
}

// Debug print host name and address
>>>>>>> 8b583dc3049d36ee176e40937169c7f1d9ee4fe5
