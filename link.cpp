/**
 * ECE408 
 * Link.cpp
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei
 * @version 0.2.0 04/21/16
 */

#include <assert.h>
#include <vector>

#include "net.h"
#include "Link.h"
#include "Packet.h"
#include "Host.h"
#include "events/event_send_pak.h"

//FUNCTIONS

// Calculate the total delay for a Packet along the Link
float Link::calcDelay(){
	// need to check for Packet! Or create one for Routers to call
	return (float)((buffer.front().first->getSize())/rate + delay);
}

// Calculate the Link rate
float Link::get_link_flow_rate() {
	// Time elapsed since last update
	float time_elapsed = simtime - update_time;
	// Flow rate is bytes sent over elapsed time (s)
	float curr_Flow_Rate = bytes_sent/time_elapsed;
	// Reset the bytes sent and most recent update time
	bytes_sent = 0;
	update_time = simtime;
	return curr_Flow_Rate;
}

// Receive Packet from Flow(Hosts)/Routers
bool Link::receive_pak(Packet *p, Node *n){
	assert((n == n1)||(n == n2));
	if(buffer.empty()){
	*errorSS << "Empty!" << occupancy << std::endl;
		// Initiate Packet transmission by inserting into buffer and priority queue
		// Stamp destination
		buffer.push(std::make_pair(p, (n == n1) ? n2 : n1));
		occupancy += p->getSize();
		float pDelay = calcDelay();
		event_send_pak *e = new event_send_pak(pDelay, this);
		Network->addEvent(e);
		e->print();
		return true;

	// Stores Packet in buffer if occupied	
	} else if (occupancy + p->getSize() <= buffer_size) {
	*errorSS << "Occupied!." << occupancy << std::endl;
		// Stamp destination
		buffer.push(std::make_pair(p, (n == n1) ? n2 : n1));
		// record buffer occupancy
		*outputSS << getName() << ", " << occupancy << ", " << simtime << ", buffer_occ" << std::endl; 
		occupancy += p->getSize();
		return true;
	// Packet dropped
	} else {
	*errorSS << "Dropped!." << std::endl;
		// record time when a Packet is dropped
		//*outputSS << getName() << ", " << simtime << ", , Packet_loss" << std::endl; 		
		delete p;
		return false; 
	}
}

// Transmit Packet from Link to next Node - Misnomer really means sent_pak
void Link::send_pak(){
	std::pair <Packet*,Node*> sent = buffer.front();
	occupancy -= (sent.first)->getSize(); // Upon receiving, the buffer is decremented
	bytes_sent += sent.first->getSize();
	(sent.second)->receive_pak(sent.first); // Currently we only have receive_pak for Hosts.
	buffer.pop(); // Pop before sending new packet
	//record Link Flowrate after Packet transmission event
	*outputSS << getName() << ", " << get_link_flow_rate() << ", " << simtime << ", Link_Flow_rate" << std::endl;  

	// repeat Packet transmission through the Link
	if(!buffer.empty()){
		float pDelay = calcDelay();
		event_send_pak *e = new event_send_pak(pDelay, this);
		Network->addEvent(e);
		e->print();
	}
	return;
}

// Used by all Nodes to send to other side of Link
Node* Link::getOtherNode(Node *n){
	return (n1 == n) ? n2 : n1;
};
