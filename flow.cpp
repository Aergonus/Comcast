/**
 * ECE408 
 * Flow.cpp
 * Purpose: 
 * 
 * @author EuiSeong Han, Eric Nguyen, Kangqiao Lei, Jaeryung Song
 * @version 0.2.0 04/21/16
 */

#include <queue>
#include <math.h>
#include <limits.h>
#include <utility>
#include "Link.h"
#include "Flow.h"
#include "Packet.h"
#include "events/event_TO.h"

// Calculates the Packet size
// Has to deal with lack Packet being possibly smaller than the max_seg_size
int Flow::calcPakSize(int currSeq){
	int nPakSize = (size - currSeq <= MAX_SEG_SIZE) ? (size - currSeq) : MAX_SEG_SIZE;
	return nPakSize;
}

// Calculates the Flow rate for logging
float Flow::get_flow_rate(){
	// Time elapsed since last update
	float time_elapsed = simtime - update_time;
	// Flow rate is bytes sent over elapsed time (s)
	float f_rate = bytes_sent/time_elapsed;
	// Reset the bytes sent and most recent update time
	bytes_sent = 0;
	update_time = simtime;
	return f_rate;
}

// Injects the Packet into the system from the Flow
Packet* Flow::send_Pak(int pakNum, int pSize, Node *pakSrc, packet_type ptype){
	Packet *p;
	// Reverses direction of incoming Packet for the newly generated Packet
	Node *pakDst = (pakSrc == src) ? dst : src;
	// Packet Generation depending on type
	if (ptype == DATA) {
		p = new data_pak(pakSrc, pakDst, ptype, KS_POISION_CONSTANT, pSize, pakNum, this);	
	} else if (ptype == ACK) {
		p = new ack_pak(pakSrc, pakDst, ptype, KS_POISION_CONSTANT, pSize, pakNum, this);
	}
	bytes_sent += p->getSize();
	// record Flowrate after Packet insertion event
	outputSS << getName() << ", " << get_flow_rate() << ", " << simtime << ", Flow_rate" << std::endl;
	// Injects the Packet into the Link
	Link->receive_pak(p, getOtherNode(pakSrc));
	return p;
}

// Sends Packets until the number of DATA Packets from this Flow in the system is equal to CWND
void Flow::send_All_Paks(){
	if (timedAck == -1) { // Is this correct?
	while(((nextSeq - sendBase)/MAX_SEG_SIZE) < CWND && !noFlow()) {
		int pakSize = calcPakSize(nextSeq);
		send_Pak(nextSeq, pakSize, dst, DATA);
		// Increments the sequence number of the next Packet to be sent by the Packet size
		nextSeq += pakSize;
		// Timeout mechanism
		if (timedAck == -1) {
			timedAck = nextSeq;
			recordTime = simtime;
		}
		dupAcks = 1;
	}
}	

// Flow initialization
// Send one Packet
void Flow::start_Flow(){
	gotAcks = 0;
	nextSeq = 0;
	sendBase = 0;
	expectedSeq = 0;
	gapDetected = false;
	dupAcks = 0;
	CWND = 1;
	ssThresh = INT_MAX;
	
	timedAck = -1;
	estRTT = 1;
	devRTT = 1;
	sampRTT = 1;
	TO = 1;
	
	int pakSize = calcPakSize(nextSeq);
	send_Pak(nextSeq, pakSize, dst, DATA);	
	nextSeq += pakSize;	//determines next DATA to be sent
	timedAck = nextSeq;	//for timeout
	recordTime = simtime;	//for timeout
	nFlow++;		//increase number of active Flows by one
	tcpTO = new event_TO(TO,&this);	//starts a timeout event
}

// Receive Packet event for Hosts
void Flow::receive_Pak(Packet *p){
	// Receiver receives the DATA
	if(p->type == DATA){
		// Would have to add 500ms timeout if waiting to send delayed acks
		// ACK generation 
		if (ackStack.empty()) {
			ackStack.push(make_pair(p->getSeqNum(),p->getAckNum())); // Doesn't this still have to generate an ACK?
		} else if(ackStack.find(p->getSeqNum()) == ackStack.begin()->second) {
			ackStack.pop();
			std::map<int,int>::iterator it;
			ackStack[p->getSeqNum()] = p->getAckNum();
			it = ackStack.find(p->getSeqNum());
			while (ackStack.find(it->second) != ackStack.end()) {
				int nAck = it->second;
				ackStack.erase(it);
				it = ackStack.find(nAck);
			}
			expectedSeq = it->second;
			send_Pak(expectedSeq, ACK_PACKET_SIZE, src, ACK);
		// Data gap detection
		} else if(ackStack.find(p->getSeqNum()) > ackStack.begin()->second) {
			// Send duplicate ack
			send_Pak(expectedSeq, ACK_PACKET_SIZE, src, ACK);
			ackStack.push(make_pair(p->getSeqNum(),p->getAckNum()));
		}

	// Transmitter receives an ACK
	} else if (p->type == ACK) {
		// Receive expected ACK number (sendbase + 1)
		if(p->getAckNum() > sendBase) {
			sendBase = p->getAckNum();
			if (sendBase >= timedAck) {
				// RTT estimation and starting the TO timer
				sampRTT = simtime - recordTime;
				estRTT = (1-ALPHA_TIMEOUT) * estRTT + ALPHA_TIMEOUT * sampRTT;
				devRTT = (1-BETA_TIMEOUT) * devRTT + BETA_TIMEOUT * abs(sampRTT - estRTT);
				TO = estRTT + 4 * devRTT;
				tcpTO->invalidate();
				tcpTO = new event_TO(TO,&this);
				timedAck = -1;
				// Estimated RTT logging
				outputSS << getFlow(&this) << ", " << estRTT << ", " << simtime << ", estimated_rtt" << std::endl; 
			}
			if (CWND >= ssThresh) {
				// Max Probing/Congestion avoidance
				if (sendBase == nextSeq - gotAcks*MAX_SEG_SIZE) {
					CWND = tcp.probeCWND(CWND);
					gotAcks = -1;
				}
				gotAcks++;
			} else {
				// Slow Start
				CWND = tcp.slowCWND(CWND);
			}
			send_All_Paks();
			dupAcks = 0;
			// Resets timer when an ACK is received
			// Is this correct? 
			tcpTO->invalidate();
			tcpTO = new event_TO(TO,&this);
		// Received a duplicate ack, increase the counter
		} else if (p->getAckNum() == sendBase) {
			dupAcks++;
			//TCP fast retransmit
			if(dupAcks == 3) {
				ssThresh = tcp.tripSS(CWND);
				CWND = tcp.tripCWND(CWND);
				send_Pak(sendBase, calcPakSize(sendBase), dst, DATA);
			// More than 3 dupAcks go into cwnd increase (doesn't send any Packets)
			} else if(dupAcks > 3) {
				if (CWND >= ssThresh) {
				// Max Probing/Congestion avoidance
					if (sendBase == nextSeq - gotAcks*MAX_SEG_SIZE) {
					CWND = tcp.probeCWND(CWND);
					gotAcks = -1;
					}
				gotAcks++;
				} else {
				// Slow Start
				CWND = tcp.slowCWND(CWND);  
				}
			}
		// Record out of order acks
		} else if (p->getAckNum() < sendBase) {
			debugSS << p->print() << std::endl; 
		}
	} 
	outputSS << getFlow(&this) << ", " << CWND << ", " << simtime << ", window_size" << std::endl;
	// Packet no longer useful, eliminate
	delete p; 
}

// Timeout occurs
void Flow::timeout_Flow() {
	ssThresh = CWND/2;
	CWND = 1;
	dupAcks = 0;
	tcpTO = new event_TO(TO,&this);
	// RETRANSMIT MISSING ACK. BUT WHICH ONE???
	send_Pak(sendBase, calcPakSize(sendBase), dst, DATA); //send a Packet
	// Debug for timeout events
	debugSS << "ACK" << (sendbase + calcPakSize(sendBase)) << " timed out at " << simtime << std::endl; //
}
