/**
 * ECE408 
 * Link.cpp
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei
 * @version 0.5.0 05/03/16
 */

#include <assert.h>
#include <vector>

#include "net.h"
#include "Link.h"
#include "Packet.h"
#include "Host.h"
#include "events/event_send_pak.h"

float Link::calcDelay(){
// Calculate the total delay for a Packet along the Link for external use
	return (float)((occupancy/rate) + delay);
}

bool Link::receive_pak(Packet *p, Node *n){
// Receive Packet from Flow(Hosts)/Routers
	assert((n == n1)||(n == n2));
	if(buffer.empty()){
		// Buffer is empty -> Store and Send immediately
		if (debug){
			*debugSS<<"BufferDebug,"<<simtime<<",";
				debugBuffer();
			*debugSS<<"Empty!,"<<simtime<<",nBufferedPackets,"<<nBuffPaks<<",Link Buffer Occupancy,"<<occupancy<<",MaxBuffSize,"<<buffer_size<<std::endl;
		}
		
		// Initiate Packet transmission by inserting into buffer and priority queue
		// Stamp destination
		buffer.push(std::make_pair(p, n));
		occupancy += p->getSize();
		if (p->getType() == DATA){
			nDataPaks++;
		} else if(p->getType() == ACK){
			nAckPaks++;
		}
		nBuffPaks++;
		logBuffer();
		
		event_send_pak *e = new event_send_pak((p->getSize()/rate), this);
		Network->addEvent(e);
		return true;
	} else if (occupancy + p->getSize() <= buffer_size){
		// Stores Packet in buffer if occupied
		if (debug){
			*debugSS<<"BufferDebug,"<<simtime<<",";
				debugBuffer();
			*debugSS<<"Occupied!,"<<simtime<<",nBufferedPackets,"<<nBuffPaks<<",Link Buffer Occupancy,"<<occupancy<<",MaxBuffSize,"<<buffer_size<<std::endl;
		}
		
		// Stamp destination
		buffer.push(std::make_pair(p, n));
		occupancy += p->getSize();
		if (p->getType() == DATA){
			nDataPaks++;
		} else if(p->getType() == ACK){
			nAckPaks++;
		}
		nBuffPaks++;
		logBuffer();
		
		if (debug){
			*debugSS<<"BufferDebug,"<<simtime<<",";
				debugBuffer();
		}
		return true;
	} else {
		// Packet dropped
		nDroppedPaks++;
		logDrops();
		if (debug){
			*debugSS<<"BufferDebug,"<<simtime<<",";
				debugBuffer();
			*debugSS<<"DroppedPak!,"<<simtime<<",";
				p->print();
		}
		delete p;
		return false;
	}
}

void Link::send_pak(){
// Transmit Packet from Link to next Node - Misnomer really means sent_pak
	std::pair <Packet*,Node*> sent = buffer.front();
	occupancy -= (sent.first)->getSize(); // Upon receiving, the buffer is decremented
	if (sent.first->getType() == DATA){
		nDataPaks--;
	} else if(sent.first->getType() == ACK){
		nAckPaks--;
	}
	nBuffPaks--;
	logBuffer();
	
	// Record Link Rate after Packet Transmission
	bytes_sent += sent.first->getSize();
	
	buffer.pop(); // Pop before sending new packet
	if (!buffer.empty()){
		// Prep send packet event
		std::pair <Packet*,Node*> toSend = buffer.front();
		float pDelay = (toSend.first->getSize()/rate) + ((sent.second->getName() != toSend.first->getDst()->getName()) ? delay:0);
		event_send_pak *e = new event_send_pak(pDelay, this);
		Network->addEvent(e);
	}
	(sent.second)->receive_pak(sent.first, this);
}

// Used by all Nodes to send to other side of Link
Node* Link::getOtherNode(Node *n){
	return (n1->getName() == n->getName()) ? n2:n1;
}

void Link::debugBuffer(){
	*debugSS<<"Buffer"<<getName()<<","<<simtime<<",nDataPaks,"<<nDataPaks<<",nAckPaks,"<<nAckPaks<<",nBuffPaks,"<<nBuffPaks<<std::endl;
}