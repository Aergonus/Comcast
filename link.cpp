/**
 * ECE408 
 * Link.cpp
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei
 * @version 0.2.0 04/21/16
 */

#include <vector>

#include "Link.h"
#include "Packet.h"
#include "Host.h"

//FUNCTIONS

// Calculate the total delay for a Packet along the Link
float Link::calcDelay(){
	// need to check for Packet! Or create one for Routers to call
	return (float)((buffer.front().first->getSize())/rate + delay);
}

// Calculate the Link rate
float Link::Link_Flow_rate() {
	// Time elapsed since last update
	float time_elapsed = simtime - update_time;
	// Flow rate is bytes sent over elapsed time (s)
	Flow_rate = bytes_sent/time_elapsed;
	// Reset the bytes sent and most recent update time
	bytes_sent = 0;
	update_time = simtime;
	return Flow_rate;
}

// Receive Packet from Flow(Hosts)/Routers
bool Link::receive_pak(Packet *p, Node *n){
	assert((n == n1)||(n == n2));
	if(buffer.empty()){
		// Initiate Packet transmission by inserting into buffer and priority queue
		// Stamp destination
		buffer.push(make_pair(p, (n == n1) ? n2 : n1));
		occupancy += p->getSize();
		float pDelay = calcDelay();
		event_send_pak e(pDelay, Network, &this);
		Network.addEvent(&e);
		return true;

	// Stores Packet in buffer if occupied	
	} else if (occupancy + p->getSize() <= buffer_size) {
		// Stamp destination
		buffer.push(make_pair(p, (n == n1) ? n2 : n1));
		// record buffer occupancy
		outputSS << getLink(&this) << ", " << occupancy << ", " << simtime << ", buffer_occ" << std::endl; 
		occupancy += p->getSize();
		return true;
	// Packet dropped
	} else {
		// record time when a Packet is dropped
		outputSS << getLink(&this) << ", " << simtime << ", , Packet_loss" << std::endl; 		
		delete p;
		return false; 
	}
}

// Transmit Packet from Link to next Node
void Link::send_pak(){
	std::pair <Packet*,Node*> sent = buffer.front();
	(sent.second)->receive_pak(sent.first); // Currently we only have receive_pak for Hosts.
	occupancy -= (sent.first)->getSize();
	bytes_sent += sent.first->getSize();
	//record Link Flowrate after Packet transmission event
	outputSS << getLink(&this) << ", " << Link_Flow_rate() << ", " << simtime << ", Link_Flow_rate" << std::endl;  
	buffer.pop();
	// repeat Packet transmission through the Link
	if(!buffer.empty()){
		float pDelay = calcDelay();
		event_send_pak e(pDelay, Network, &this); 
		Network.addEvent(&e);
	}
	return;
}

// Used by all Nodes to send to other side of Link
Node* getOtherNode(Node *n){
	return (n1 == n) ? n2 : n1;
};
