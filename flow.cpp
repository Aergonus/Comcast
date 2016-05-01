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

#include "Flow.h"
#include "net.h"
#include "Node.h"
#include "Link.h"
#include "Packet.h"
#include "events/event_TO.h"

// Calculates the Packet size
// Has to deal with lack Packet being possibly smaller than the max_seg_size
int Flow::calcPakSize(int currSeq){
	int nPakSize = (size - currSeq <= MAX_SEG_SIZE) ? (size - currSeq) : MAX_SEG_SIZE;
	*debugSS << "	Calculating Next Packet currSeq " << currSeq << ", nPakSize " << nPakSize << std::endl;
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
		p = new data_pak(pakSrc, pakDst, pSize, KS_POISION_CONSTANT, ptype, pakNum, this);	
	} else if (ptype == ACK) {
		p = new ack_pak(pakSrc, pakDst, pSize, KS_POISION_CONSTANT, ptype, pakNum, this);
	}
	// Injects the Packet into the Link
	*debugSS << "Created in Flow sendpak\t";
	p->print();
	pakSrc->getLinks().front()->receive_pak(p, pakSrc->getConnectedNode(pakSrc->getLinks().front()));
	return p;
}

// Sends Packets until the number of DATA Packets from this Flow in the system is equal to CWND
void Flow::send_All_Paks(){
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
	dupAcks = 0;
	CWND = 1;
	ssThresh = INT_MAX;
	
	timedAck = -1;
	estRTT = 1;
	devRTT = 1;
	sampRTT = 1;
	TO = 1;
	int pakSize = calcPakSize(nextSeq);
	send_Pak(nextSeq, pakSize, src, DATA);	
	nextSeq += pakSize;	//determines next DATA to be sent
	timedAck = nextSeq;	//for timeout
	recordTime = simtime;	//for timeout
	tcpTO = new event_TO(TO,this);
	Network->addEvent(tcpTO);	//starts a timeout event
	*debugSS << "Created ";
	tcpTO->print();
}

// Receive Packet event for Hosts
void Flow::receive_Pak(Packet *p){

	// Receiver receives the DATA
	if(p->type == DATA){
		// Would have to add 500ms timeout if waiting to send delayed acks
		*debugSS << "Got Data" << std::endl;
		p->print();
		// ACK generation 
		if (ackStack.empty()) {
			*debugSS << "Empty AckStack" << std::endl;
			ackStack.insert(std::make_pair(p->getSeqNum(),p->getAckNum())); 
			expectedSeq = p->getAckNum();
			send_Pak(expectedSeq, ACK_PACKET_SIZE, src, ACK);
		} else if(p->getSeqNum() == ackStack.begin()->second) {
/*
			*debugSS << "Pushed AckStack" << std::endl;
				for (const auto &p : ackStack) {
					*debugSS << "\tackStack[" << p.first << "] = " << p.second << std::endl;
				}
*/
			ackStack.erase(ackStack.begin());
			std::map<int,int>::iterator it;
			ackStack[p->getSeqNum()] = p->getAckNum();
			it = ackStack.find(p->getSeqNum());
			while (ackStack.find(it->second) != ackStack.end()) {
				int nAck = it->second;
				ackStack.erase(it);
				it = ackStack.find(nAck);
			}
			expectedSeq = it->second;
/*
			*debugSS << "Popped AckStack" << std::endl;
				for (const auto &p : ackStack) {
					*debugSS << "\tackStack[" << p.first << "] = " << p.second << std::endl;
				}
*/
			send_Pak(expectedSeq, ACK_PACKET_SIZE, src, ACK);
			
		// Data gap detection
		} else if(p->getSeqNum() > ackStack.begin()->second) {
			*debugSS << "Gap detected!" << std::endl;
/*
				for (const auto &p : ackStack) {
					*debugSS << "\tackStack[" << p.first << "] = " << p.second << std::endl;
				}
*/
			ackStack.insert(std::make_pair(p->getSeqNum(),p->getAckNum()));
			// Send duplicate ack
			send_Pak(expectedSeq, ACK_PACKET_SIZE, src, ACK);
		}

	// Transmitter receives an ACK
	} else if (p->type == ACK) {
		*debugSS << "Got Ack" << std::endl;
		p->print();
		// Receive expected ACK number (sendbase + 1)
		if(p->getAckNum() > sendBase) {
			*debugSS << "Expected ack received. sendBase " << sendBase << " ackNum " << p->getAckNum() << std::endl;
			sendBase = p->getAckNum();
			// Check if Flow finished
			if (sendBase == size) {
				*debugSS << "Flow finished! " << getName() << std::endl;
				Network->FlowFinished();
				//Network->vomitEvents();
				bytes_sent += p->getSize();
				// record Flowrate after Packet insertion event
				*outputSS << getName() << ", " << get_flow_rate() << ", " << simtime << ", Flow_rate" << std::endl;
				*outputSS << getName() << ", " << CWND << ", " << simtime << ", window_size" << std::endl;
				// Packet no longer useful, eliminate
				p->print();
				delete p; 
				return;
			}
			if (sendBase >= timedAck) {
				// RTT estimation and starting the TO timer
				sampRTT = simtime - recordTime;
				estRTT = (1-ALPHA_TIMEOUT) * estRTT + ALPHA_TIMEOUT * sampRTT + .1; // Add 100 ms to avoid converge
				devRTT = (1-BETA_TIMEOUT) * devRTT + BETA_TIMEOUT * abs(sampRTT - estRTT);
				TO = estRTT + 4 * devRTT;
				*debugSS << "BOOM A: SampRTT" << sampRTT << " TO " << TO << std::endl;
				timedAck = -1;
				// Estimated RTT logging
				*outputSS << getName() << ", " << estRTT << ", " << simtime << ", estimated_rtt" << std::endl; 
			}
			if (CWND >= ssThresh) {
				*debugSS << "BOOM B CWND" << CWND << "SSTHRESH " << ssThresh << " GotACKS " << gotAcks << std::endl;
				// Max Probing/Congestion avoidance
				if (gotAcks == CWND) {//(sendBase == nextSeq - gotAcks*MAX_SEG_SIZE) {
					algo->probeCWND(&CWND);
					gotAcks = -1;
				}
				gotAcks++;
			} else {
				*debugSS << "BOOM C CWND" << CWND << "SSTHRESH " << ssThresh << std::endl;
				// Slow Start
				algo->slowCWND(&CWND);
				*debugSS << "BOOM C CWND" << CWND << "SSTHRESH " << ssThresh << std::endl;
			}
			send_All_Paks();
			dupAcks = 0;
			// Resets timer when an ACK is received
			tcpTO->invalidate();
			tcpTO = new event_TO(TO,this);
			Network->addEvent(tcpTO);
			*debugSS << "Created ";
			tcpTO->print();

		// Received a duplicate ack, increase the counter
		} else if (p->getAckNum() == sendBase) {
			*debugSS << "Dup Ack " << sendBase << std::endl;
			dupAcks++;
			//TCP fast retransmit
			if(dupAcks == 3) {
				*debugSS << "Fast Retransmit!" << std::endl;
				algo->tripSS(&CWND, &ssThresh);
				algo->tripCWND(&CWND);
				int pakSize = calcPakSize(sendBase);
				send_Pak(sendBase, pakSize, dst, DATA);
				nextSeq = sendBase + pakSize;
				tcpTO->invalidate();
				tcpTO = new event_TO(TO,this);
				Network->addEvent(tcpTO);
				*debugSS << "Created ";
				tcpTO->print();
			// More than 3 dupAcks go into cwnd increase (doesn't send any Packets)
			} else if(dupAcks > 3) {
				if (CWND >= ssThresh) {
				// Max Probing/Congestion avoidance
				//*outputSS << "Probing," << CWND << "," << gotAcks << ",CWND" << std::endl;
				*debugSS << "Probing CWND" << CWND << "SSTHRESH " << ssThresh << " GotACKS " << gotAcks << std::endl;
				this->print();
					if (gotAcks == CWND) {//(sendBase == nextSeq - gotAcks*MAX_SEG_SIZE) {
						algo->probeCWND(&CWND);
						gotAcks = -1;
					}
				gotAcks++;
				} else {
				// Slow Start
					//*outputSS << "Slow," << CWND << "," << gotAcks << ",CWND" << std::endl;
					algo->slowCWND(&CWND);  
				}
				/*
				send_All_Paks();
				// Resets timer when an ACK is received
				tcpTO->invalidate();
				tcpTO = new event_TO(TO,this);
				Network->addEvent(tcpTO);
				*debugSS << "Created ";
				tcpTO->print();
				*/
			}
			
		// Record out of order acks
		} else if (p->getAckNum() < sendBase) {
			*debugSS << "OUT OF ORDER " << p->getAckNum() << std::endl; 
		}
		//record window size
		*outputSS << getName() << ", " << CWND << ", " << simtime << ", window_size" << std::endl;
	} 
	bytes_sent += p->getSize();
	// record Flowrate after Packet insertion event
	*outputSS << getName() << ", " << get_flow_rate() << ", " << simtime << ", Flow_rate" << std::endl;
	// Packet no longer useful, eliminate
	*debugSS << "Processed ";
	p->print();
	delete p; 
}

// Timeout occurs
void Flow::timeout_Flow() {
	ssThresh = CWND/2;
	CWND = 1;
	dupAcks = 0;
	tcpTO->invalidate();
	tcpTO = new event_TO(tcpTO->getTO()*2,this);
	Network->addEvent(tcpTO);
	int pakSize = calcPakSize(sendBase);
	send_Pak(sendBase, pakSize, dst, DATA);
	nextSeq = sendBase + pakSize;

	// Debug for timeout events
	*debugSS << "ACK" << (sendBase + calcPakSize(sendBase)) << " timed out at " << simtime << std::endl; //
}

void Flow::print() {
	*debugSS << "Flow " << getName() << ". nextSeq " << nextSeq << " . sendBase " << sendBase << ". dupAcks " << dupAcks 
		<< ". CWND " << CWND << ". ssThresh " << ssThresh << ". gotAcks" << gotAcks
		<< ". estRTT" << estRTT << ". devRTT" << devRTT << ". sampRTT" << sampRTT << ". TO " << TO 
		<< std::endl;
}
