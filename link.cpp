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
	return (float)((occupancy/rate) + delay);// *nBuffPaks); // Fuck this lol *nFlips has a bug too //TODO:
}

// Receive Packet from Flow(Hosts)/Routers
bool Link::receive_pak(Packet *p, Node *n){
	assert((n == n1)||(n == n2));
	if(buffer.empty()){
	// Buffer is empty -> Store and Send
#ifndef NDEBUG
if (debug) {
	*debugSS << "BufferDebug,"<<simtime<<",";
		debugBuffer();
	*debugSS<<"Empty!,"<<simtime<<",nBufferedPackets,"<<nBuffPaks<<",Link Buffer Occupancy,"<<occupancy<<",MaxBuffSize,"<<buffer_size<<std::endl;
}
#endif
		// Initiate Packet transmission by inserting into buffer and priority queue
		// Stamp destination
		buffer.push(std::make_pair(p, (n == n1) ? n2 : n1));
		occupancy += p->getSize();
		if (p->getSeqNum() != -1) {
				direction = true;
			nDataPaks++;
		} else {
				direction = false;
			nAckPaks++;
		}
		nBuffPaks++;
		logBuffer();
		
#ifndef NDEBUG
if (debug) {
	*debugSS << "BufferDebug,"<<simtime<<",";
		debugBuffer();
}
#endif
		float pDelay = calcDelay();
		event_send_pak *e = new event_send_pak(pDelay, this);
		Network->addEvent(e);
#ifndef NDEBUG
if (debug) {
	*debugSS << "CreateEvent,"<<simtime<<",Created in Link receivepak,";
		e->print();
}
#endif
		return true;
	} else if (occupancy + p->getSize() <= buffer_size) {
	// Stores Packet in buffer if occupied	
#ifndef NDEBUG
if (debug) {
	*debugSS << "BufferDebug,"<<simtime<<",";
		debugBuffer();
	*debugSS<<"Occupied!,"<<simtime<<",nBufferedPackets,"<<nBuffPaks<<",Link Buffer Occupancy,"<<occupancy<<",MaxBuffSize,"<<buffer_size<<std::endl;
}
#endif
		// Stamp destination
		buffer.push(std::make_pair(p, (n == n1) ? n2 : n1));
		occupancy += p->getSize();
		if (p->getSeqNum() != -1) {
			if (!direction) {
				direction = true;
				nFlips++;
			}
			nDataPaks++;
		} else {
			if (direction) {
				direction = false;
				nFlips++;
			}
			nAckPaks++;
		}
		nBuffPaks++;
		logBuffer();
		
#ifndef NDEBUG
if (debug) {
	*debugSS << "BufferDebug,"<<simtime<<",";
		debugBuffer();
}
#endif

		float pDelay = calcDelay();
		// record buffer occupancy
#ifndef NDEBUG
if (debug) {
	*debugSS<<"BufferedPak!,"<<simtime<<",nBufferedPackets,"<<nBuffPaks<<",Link Buffer Occupancy,"<<occupancy<<",MaxBuffSize,"<<buffer_size<<std::endl;
}
#endif
		event_send_pak *e = new event_send_pak(pDelay, this);
		Network->addEvent(e);
#ifndef NDEBUG
if (debug) {
	*debugSS << "CreateEvent,"<<simtime<<",Created in Link receivepak,";
		e->print();
}
#endif
		return true;
	} else {
		// Packet dropped
		nDroppedPaks++;
		logDrops();
		
		// record time when a Packet is dropped
#ifndef NDEBUG
if (debug) {
	*debugSS << "BufferDebug,"<<simtime<<",";
		debugBuffer();
	*debugSS << "DroppedPak!," << simtime << ",";
		p->print();
}
#endif
		delete p;
		return false; 
	}
}

// Transmit Packet from Link to next Node - Misnomer really means sent_pak
void Link::send_pak(){
	std::pair <Packet*,Node*> sent = buffer.front();
	occupancy -= (sent.first)->getSize(); // Upon receiving, the buffer is decremented
	if (sent.first->getSeqNum() != -1) {
		if (!direction) {
			direction = true;
			nFlips--;
		}
		nDataPaks--;
	} else {
		if (direction) {
			direction = false;
			nFlips--;
		}
		nAckPaks--;
	}
	nBuffPaks--;
	logBuffer();
	
	// Record Link Rate after Packet Transmission
	bytes_sent += sent.first->getSize();
	logLinkRate();
	
	buffer.pop(); // Pop before sending new packet
	(sent.second)->receive_pak(sent.first);
}

// Used by all Nodes to send to other side of Link
Node* Link::getOtherNode(Node *n){
	return (n1 == n) ? n2 : n1;
}

void Link::debugBuffer(){
	*debugSS << "Buffer" << getName() << "," << simtime << ",nDataPaks," << nDataPaks << ",nAckPaks," << nAckPaks << ",nBuffPaks," << nBuffPaks << std::endl;
}