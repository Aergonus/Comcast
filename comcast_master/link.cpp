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

// Calculate the total delay for a packet along the link
float link::calcDelay(){
	// need to check for packet! Or create one for Routers to call
	return (float)((buffer.front().first->getSize())/rate + delay);
}

// Calculate the link rate
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
bool link::receive_packet(packet *p, node *n){
	assert((n == n1)||(n == n2));
	if(buffer.empty()){
		// Initiate packet transmission by inserting into buffer and priority queue
		// Stamp destination
		buffer.push(make_pair(p, (n == n1) ? n2 : n1));
		occupancy += p->getSize();
		float pDelay = calcDelay();
		event_send_pak e(pDelay, Network, &this);
		Network.addEvent(&e);
		return true;

	// Stores packet in buffer if occupied	
	} else if (occupancy + p->getSize() <= buffer_size) {
		// Stamp destination
		buffer.push(make_pair(p, (n == n1) ? n2 : n1));
		// record buffer occupancy
		outputSS << getLink(&this) << ", " << occupancy << ", " << simtime << ", buffer_occ" << std::endl; 
		occupancy += p->getSize();
		return true;
	// packet dropped
	} else {
		// record time when a packet is dropped
		outputSS << getLink(&this) << ", " << simtime << ", , packet_loss" << std::endl; 		
		delete p;
		return false; 
	}
}

// Transmit packet from link to next node
void link::send_pak(){
	std::pair <packet*,node*> sent = buffer.front();
	(sent.second)->receive_pak(sent.first); // Currently we only have receive_pak for hosts.
	occupancy -= (sent.first)->getSize();
	bytes_sent += sent.first->getSize();
	//record link flowrate after packet transmission event
	outputSS << getLink(&this) << ", " << link_flow_rate() << ", " << simtime << ", link_flow_rate" << std::endl;  
	buffer.pop();
	// repeat packet transmission through the link
	if(!buffer.empty()){
		float pDelay = calcDelay();
		event_send_pak e(pDelay, Network, &this); 
		Network.addEvent(&e);
	}
	return;
}

// Used by all nodes to send to other side of link
node* getOtherNode(node *n){
	return (n1 == n) ? n2 : n1;
<<<<<<< HEAD
}

bool hasNode(node* n){
  return (*n == *n1 || *n == *n2);
}

=======
};
>>>>>>> 8b583dc3049d36ee176e40937169c7f1d9ee4fe5
