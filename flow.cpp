/**
 * ECE408 
 * flow.cpp
 * Purpose: 
 * 
 * @author EuiSeong Han, Eric Nguyen, Kangqiao Lei, Jaeryung Song
 * @version 0.1.8 04/21/16
 */

#include <queue>
#include <math.h>
#include <limits.h>
#include "flow.h"
#include "packet.h"
#include "event_TO.h"
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

void send_All_Paks(){
	if (timedAck = -1) {
		// pak about to be sent, get the ack #, nseq#+pakSize, set nextseq
	while(((nextSeq - sendBase)/FLOW_PACKET_SIZE) < CWND && !noflow()) {
		int pakSize = calcPakSize(nextSeq);
		send_Pak(nextSeq, pakSize, dst, DATA);
		nextSeq += pakSize;
		if (timedAck = -1) {
			timedAck = nextSeq;
			// record time
		}
		dupAcks = 1;
	}
}

void start_Flow(){
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
	
	// Poke TCP and send event *tcpTO, i.e. start timeout for TCP
	int pakSize = calcPakSize(nextSeq);
	send_Pak(nextSeq, pakSize, dst, DATA);
	nextSeq += pakSize;
	timedAck = nextSeq;
	recordTime = time;
	tcpTO = new event_TO(TO,&this);
}

void receive_Pak(packet *p){
	if(p->type == DATA){
	// Receiver
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
	// Transmitter //TODO: Selective repeat? sliding window? USE ACKSTACK
		// Tell TCP that ack received
		if(p->getAckNum() > sendBase) {
			sendBase = p->getAckNum();
			if (sendBase >= timedAck) {
				sampRTT = recordTime - timedAck;
				estRTT = (1-ALPHA_TIMEOUT) * estRTT + ALPHA_TIMEOUT * sampRTT;
				devRTT = (1-BETA_TIMEOUT) * devRTT + BETA_TIMEOUT * abs(sampRTT - estRTT);
				TO = estRTT + 4 * devRTT;
				tcpTO->invalidate();
				tcpTO = new event_TO(TO,&this);
				timedAck = -1;
			}
			if (CWND >= ssThresh) {
				// Max Probing/Congestion avoidance
				if (sendBase == nextSeq - gotAcks*FLOW_PACKET_SIZE) {
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
			// Reset timer;
		} else if (p->getAckNum() = sendBase) {
			dupAcks++;
			if(dupAcks == 3) {
				//TCP fast retransmit
				ssThresh = tcp.tripSS(CWND);
				CWND = tcp.tripCWND(CWND);
				send_Pak(sendBase, calcPakSize(sendBase), dst, DATA);
			} else {
				CWND = tcp.fastCWND(CWND);
				send_All_Paks();
			}
		} else if (p->getAckNum() < sendBase) {
			// Log ignored ack @Namu
		}
	} 
	delete p;
}

void flow_Timeout() {
	ssThresh = CWND/2;
	CWND = 1;
	dupAcks = 0;
	tcpTO = new event_TO(TO,&this);
	// RETRANSMIT MISSING ACK. BUT WHICH ONE???
}