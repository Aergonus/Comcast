/**
 * ECE408 
 * Router.cpp
 * Purpose: 
 * 
 * @author Eui Han, Arvind Nagalingam, Kangqiao Lei
 * @version 0.2.0 04/19/16
 */
 
#include "Link.h"
#include "Router.h"

void Router::receive_pak(Packet *p, Link *l){
  /*
  if (p->type == ROUTING){ //if the received Packet is control type
	receiveControl(p);
  }
  else{
	get_route(p.getDst()) -> receive_pak(p); //pushes the Packet to the Link
  }
  */
  
  // Only have else for test case 0.
  
} 

// Gets the other Node connected to the Link
Node* Router::getConnectedNode(Link *connection){
	return connection->getOtherNode(this);
}

void Router::print() {

}