/**
 * ECE408 
 * flow.cpp
 * Purpose: 
 * 
 * @author EuiSeong Han, Eric Nguyen, Kangqiao Lei, Jaeryung Song
 * @version 0.1.7.5 04/19/16
 */

#include <queue>
#include <limits.h>
#include "flow.h"
#include "packet.h"
#include "util.h"

int calcPakSize(int currSeq){
	int nPakSize = (size - currSeq <= FLOW_PACKET_SIZE) ? (size - currSeq) : FLOW_PACKET_SIZE;
	return nPakSize;
}

packet* send_Pak(int pakNum, int pSize, node *pakSrc, packet_type ptype){
	packet p*;
	node *pakDst = (n == src) ? dst : src;
	if (ptype == DATA) {
		p = new data_pak(pakSrc, pakDst, ptype, KS_POISION_CONSTANT, pSize, pakNum, &this);
	} else if (ptype == ACK {
		p = new ack_pak(pakSrc, pakDst, ptype, KS_POISION_CONSTANT, pSize, pakNum, &this);
	}
	link->receive_pak(p, n);
	return p;
}

void start_Flow(){
	nextSeq = 0;
	sendBase = 0;
	expectedSeq = 0;
	gapDetected = false;
	dupAcks = 1;
	CWND = 1;
	ssThresh = INT_MAX;
	
	estRTT = 1;
	devRTT = 1;
	sampRTT = 1;
	TO = 1;
	
	// Poke TCP and send event *tcpTO, i.e. start timeout for TCP
	send_Pak(nextSeq, calcPakSize(nextSeq), dst, DATA);
	nextSeq += pakSize;
	ackStack.push(nextSeq);
}

void receive_Pak(packet *p){
	if(p->type == DATA){
		// Would have to add 500ms timeout if waiting to send delayed acks
		if(p->getSeqNum() > expectedSeq) {
			send_Pak(expectedSeq, ACK_PACKET_SIZE, src, ACK);
			if(!gapDetected) {
				gapDectected = true;
				maxGapSeq = p->getAckNum();
			}
		} else {
			send_Pak(p->getAckNum(), ACK_PACKET_SIZE, src, ACK);
			expectedSeq += p->getSize;
			if(gapDetected && expectedSeq >= maxGapSeq) {
				gapDetected = false;
			}
		}
	} else if (p->type == ACK) {
		// Tell TCP that ack received
		if(p->getAckNum() > sendBase) {
			sendBase = p->getAckNum();
			send_Pak(nextSeq, calcPakSize(nextSeq), dst, DATA);
			nextSeq += pakSize;
			dupAcks = 1;
			// Reset timer;
		} else if (p->getAckNum() = sendBase) {
			dupAcks += 1;
			if(dupAcks == 3) {
				//TCP fast retransmit
				send_Pak(sendBase, calcPakSize(sendBase), dst, DATA);
				nextSeq = sendBase + pakSize; // Go-Back-N
			}
		}
	} 
	delete p;
}

// if timer timeout retransmit not-yet-acked seg with smalles seq #, make new timeout event

// If loss, set ssThresh to CWND/2
// TCP
// If CWND >= ssThresh, then no longer slow start but congestion avoidance
// Increment cwnd for ack, and check to send packet if not ignoring
// 