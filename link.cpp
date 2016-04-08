/**
 * ECE408 
 * link.cpp
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei
 * @version 0.1.5 04/05/16
 */

#include <vector>

#include "link.h"
#include "packet.h"
#include "host.h"

//FUNCTIONS

float link::calcDelay(){
	return (float)((buffer.front().first->getSize())/rate + delay);
}

bool link::receive_pak(packet *p, node *n){
	assert((n == n1)||(n == n2));
	if(buffer.empty()){
		// Stamp destination 
		buffer.push(make_pair(p, (n == n1) ? n2 : n1));
		occupancy += p->getSize();
		float pDelay = calcDelay();
/* TODODODODODODODODODODODODO ****!!!*!*!*!*!*!*!*
		event aksjdflakjsld;fj 
		Network.addEvent(...);
*/
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
/* TODODODODODODODODODODODODO ****!!!*!*!*!*!*!*!*
		event aksjdflakjsld;fj 
		Network.addEvent(...);
*/
	}
	return;
}

node* getOtherNode(node *n){
	return (n1 == n) ? n2 : n1;
};