/**
 * ECE408 
 * link.cpp
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei
 * @version 0.2.0 04/21/16
 */

#include <vector>

#include "link.h"
#include "packet.h"
#include "host.h"

//FUNCTIONS

float link::calcDelay(){
	// need to check for packet! Or create one for Routers to call
	return (float)((buffer.front().first->getSize())/rate + delay);
}

bool link::receive_pak(packet *p, node *n){
	assert((n == n1)||(n == n2));
	if(buffer.empty()){
		// Stamp destination 
		buffer.push(make_pair(p, (n == n1) ? n2 : n1));
		occupancy += p->getSize();
		float pDelay = calcDelay();
		event_send_pak e(pDelay, Network, &this);
		Network.addEvent(&e);
		return true;
	} else if (occupancy + p->getSize() <= buffer_size) {
		// Stamp destination 
		buffer.push(make_pair(p, (n == n1) ? n2 : n1));
		occupancy += p->getSize();
		return true;
	} 
	// Packet dropped from the universe so deallocate
	delete p;
	return false;
}

void link::send_pak(){
	std::pair <packet*,node*> sent = buffer.front();
	(sent.second)->receive_pak(sent.first);
	occupancy -= (sent.first)->getSize();
	buffer.pop();
	if(!buffer.empty()){
		float pDelay = calcDelay();
		event_send_pak e(pDelay, Network, &this);
		Network.addEvent(&e);
	}
	return;
}

node* getOtherNode(node *n){
	return (n1 == n) ? n2 : n1;
};
