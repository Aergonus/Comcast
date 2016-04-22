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

float Link::link_flow_rate() {
	// Time elapsed since last update
	float time_elapsed = simtime - update_time;
	// Flow rate is bytes sent over elapsed time (s)
	flow_rate = bytes_sent/time_elapsed;
	// Reset the bytes sent and most recent update time
	bytes_sent = 0;
	update_time = simtime;
	return flow_rate;
}

// Receive packet from flow(hosts)/routers
bool link::receive_pak(packet *p, node *n){
	assert((n == n1)||(n == n2));
	if(buffer.empty()){
		// Stamp destination 
		buffer.push(make_pair(p, (n == n1) ? n2 : n1));
		occupancy += p->getSize();
		float pDelay = calcDelay();
		event_send_pak e(pDelay, Network, &this); // Put into priority q
		Network.addEvent(&e);
		return true;
	} else if (occupancy + p->getSize() <= buffer_size) {
		// Stamp destination
		buffer.push(make_pair(p, (n == n1) ? n2 : n1));
		outputSS << getLink(&this) << ", " << occupancy << ", " << simtime << ", buffer_occ" << endl; //record buffer occupancy
		occupancy += p->getSize();
		return true;
	} else {
		outputSS << getLink(&this) << ", " << simtime << ", , packet_loss" << endl; //record time when a packet was dropped		
		delete p;
		return false; //packet dropped
	}
}

// Send packet from link to next node
void link::send_pak(){
	std::pair <packet*,node*> sent = buffer.front();
	(sent.second)->receive_pak(sent.first);
	occupancy -= (sent.first)->getSize();
	bytes_sent += sent.first->getSize();
	outputSS << getLink(&this) << ", " << link_flow_rate() << ", " << simtime << ", link_flow_rate" << endl; //record link flowrate after packet transmission event 
	buffer.pop();
	if(!buffer.empty()){
		float pDelay = calcDelay();
		event_send_pak e(pDelay, Network, &this); // repeat packet transmission through the link
		Network.addEvent(&e);
	}
	return;
}

// Used by all nodes to send to other side of link
node* getOtherNode(node *n){
	return (n1 == n) ? n2 : n1;
};
