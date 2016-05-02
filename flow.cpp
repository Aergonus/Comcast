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
	*debugSS << "CalcNextPak,"<<simtime<<",seqNum,"<<currSeq << ",ackNum," << nPakSize << std::endl;
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
	*debugSS << "CreatePak,"<<simtime<<",Created in Flow sendpak,";
	p->print();
	// Calls Link's Recieve Packet (p,nextNodeHop)
	pakSrc->getLinks().front()->receive_pak(p, pakSrc->getConnectedNode(pakSrc->getLinks().front()));
	pakSrc->getLinks().front()->debugBuffer();
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
		if (timedAck == -1 && nextSeq > maxSeqSent) {
			timedAck = nextSeq;
			recordTime = simtime;
			*debugSS << "TimedACK,"<<simtime<<",nSeqNum,"<<timedAck<<",recordTime,"<<recordTime<<std::endl;
		}
		maxSeqSent = nextSeq > maxSeqSent ? nextSeq : maxSeqSent;
	}
}

// Flow initialization
// Send one Packet
void Flow::start_Flow(){
	gotAcks = 1;
	nextSeq = 0;
	sendBase = 0;
	expectedSeq = 0;
	dupAcks = 0;
	CWND = 1;
	ssThresh = 64;
	
	// Log CWND at start. Do not plot ssThresh since it is not initialized yet
	logCWND();
	
	timedAck = -1;
	estRTT = 1;
	devRTT = 1;
	sampRTT = 1;
	TO = 1;
	
	// Log RTT calcs and TO at start
	logRTTO();
	
	// Transmit First Packet
	int pakSize = calcPakSize(nextSeq);
	send_Pak(nextSeq, pakSize, src, DATA);	
	nextSeq += pakSize;	//determines next DATA to be sent
	maxSeqSent = nextSeq > maxSeqSent ? nextSeq : maxSeqSent;
	
	// Timeout Initializations
	timedAck = nextSeq;
	recordTime = simtime;
	
	// Create Timeout Event
	tcpTO = new event_TO(TO,this);
	Network->addEvent(tcpTO);
	*debugSS << "CreateEvent," << simtime << ",";
	tcpTO->print();
}

// Receive Packet event for Hosts
void Flow::receive_Pak(Packet *p){ 
	if(p->type == DATA){
	// Receiver receives the DATA
		// Would have to add 500ms timeout if waiting to send delayed acks
		*debugSS << "GotData,"<<simtime<<",";
		p->print();

		// ACK generation 
		if (ackStack.empty()) {
			*debugSS << "EmptyAckStack,"<<simtime<< std::endl;
			ackStack.insert(std::make_pair(p->getSeqNum(),p->getAckNum())); 
			expectedSeq = p->getAckNum();
			send_Pak(expectedSeq, ACK_PACKET_SIZE, src, ACK);
		} else if(p->getSeqNum() == ackStack.begin()->second) {
/*
			*debugSS << "Pushed AckStack,"<<simtime;
				for (const auto &p : ackStack) {
					*debugSS << ",ackStack[" << p.first << "] = " << p.second;
				}
			*debugSS << std::endl;
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
			*debugSS << "Pushed AckStack,"<<simtime;
				for (const auto &p : ackStack) {
					*debugSS << ",ackStack[" << p.first << "] = " << p.second;
				}
			*debugSS << std::endl;
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
		} else {
			// Got something lower, send back expected ack
			send_Pak(expectedSeq, ACK_PACKET_SIZE, src, ACK);
		}
	} else if (p->type == ACK) {
		// Transmitter receives an ACK
		if(p->getAckNum() > sendBase) {
			// Receive expected ACK number (sendbase + 1)
			*debugSS << "ExpectedAckReceived,"<<simtime<<",";
			p->print();
			sendBase = p->getAckNum();
			if (sendBase == size) {
				// Check if Flow finished
				*debugSS << "Processed," << simtime << ",";
				p->print();
				
				*debugSS << "FlowFinished," << simtime << getName() << std::endl;
				Network->FlowFinished();
				//Network->vomitEvents();
				bytes_sent += p->getSize();
				// record Flowrate after Packet insertion event
				//*outputSS << getName() << ", " << get_flow_rate() << ", " << simtime << ", Flow_rate" << std::endl;
				
				// Logging at end of flow
				logCWND();
				logSSThresh();
				logRTTO();
				
				// Packet no longer useful, eliminate
				delete p; 
				return;
			}
			if (sendBase >= timedAck && timedAck != -1) {
				// RTT estimation and starting the TO timer
				sampRTT = simtime - recordTime;
				estRTT = (1-ALPHA_TIMEOUT) * estRTT + ALPHA_TIMEOUT * sampRTT; 
				devRTT = (1-BETA_TIMEOUT) * devRTT + BETA_TIMEOUT * abs(sampRTT - estRTT);
				TO = estRTT + 4 * devRTT + 0.100; // Add 100 ms to avoid converge
				logRTTO();
				
				*debugSS << "UpdateRTT," << simtime << "," << getName() << ",sampRTT," << sampRTT << ",TO," << TO << std::endl;
				timedAck = -1;
			}
			if (dupAcks > 3) {
				if (ssThresh != INT_MAX) {logSSThresh();}
				algo->fastRecovery(&CWND, &ssThresh);
				logCWND();
				logSSThresh();
				gotAcks = 1;
			} else if (CWND >= ssThresh) {
				*debugSS << "CongestAvoid," << simtime << ",CWND," << CWND << ",SSTHRESH," << ssThresh << ",GotACKS," << gotAcks << std::endl;
				// Max Probing/Congestion avoidance
				if (gotAcks == CWND) {//(sendBase == nextSeq - gotAcks*MAX_SEG_SIZE) {
					algo->probeCWND(&CWND);
					logCWND();
					gotAcks = 0;
				}
				gotAcks++;
			} else {
				// Slow Start
				algo->slowCWND(&CWND);
				logCWND();
			}
			send_All_Paks();
			dupAcks = 0;
			// Resets timer when an ACK is received
			logRTTO();
			tcpTO->invalidate();
			tcpTO = new event_TO(TO,this);
			Network->addEvent(tcpTO);
			*debugSS << "CreateEvent," << simtime << ",";
			tcpTO->print();
		} else if (p->getAckNum() == sendBase) {
			// Received a duplicate ack, increase the counter
			*debugSS << "Dup Ack " << sendBase << std::endl;
			dupAcks++;
			if(dupAcks == 3) {
			//TCP fast retransmit
				if (ssThresh != INT_MAX) {logSSThresh();}
				algo->fastRetransmit(&CWND, &ssThresh);
				logCWND();
				logSSThresh();
				
				int pakSize = calcPakSize(sendBase);
				send_Pak(sendBase, pakSize, dst, DATA);
				
				logRTTO();
				tcpTO->invalidate();
				tcpTO = new event_TO(TO,this);
				Network->addEvent(tcpTO);
				*debugSS << "CreateEvent," << simtime << ",";
				tcpTO->print();
			} else if(dupAcks > 3) {
				// More than 3 dupAcks go into cwnd increase
				algo->tripDups(&CWND);
				logCWND();
				send_All_Paks();
			}
			
		// Record out of order acks
		} else if (p->getAckNum() < sendBase) {
			*debugSS << "OUT OF ORDER " << p->getAckNum() << std::endl; 
		}
		//record window size
		//*outputSS << getName() << ", " << CWND << ", " << simtime << ", window_size" << std::endl;
	}
	bytes_sent += p->getSize();
	// record Flowrate after Packet insertion event
	//*outputSS << getName() << ", " << get_flow_rate() << ", " << simtime << ", Flow_rate" << std::endl;
	// Packet no longer useful, eliminate
	*debugSS << "Processed," << simtime << ",";
	p->print();
	delete p; 
}

// Timeout occurs
void Flow::timeout_Flow() {
	// Debug for timeout events
	*debugSS << "Timeout," << simtime << ",AckNumTO," <<(sendBase + calcPakSize(sendBase)) << ",oldCWND," << CWND << ",oldSSThresh," << ssThresh << std::endl;
	if (ssThresh != INT_MAX) {logSSThresh();}
	ssThresh = (CWND / 2) < 2 ? 2 : (CWND / 2);
	CWND = 1;
	logCWND();
	logSSThresh();
	
	//dupAcks = 0;
	timedAck = -1;
	estRTT *= 2;
	TO = estRTT + 4 * devRTT + + 0.100; // Add 100 ms to avoid converge
	logRTTO();
	
	tcpTO->invalidate();
	tcpTO = new event_TO(TO,this);
	Network->addEvent(tcpTO);
	*debugSS << "CreateEvent," << simtime << ",";
	tcpTO->print();
	int pakSize = calcPakSize(sendBase);
	send_Pak(sendBase, pakSize, dst, DATA);
	nextSeq = sendBase + pakSize;
	maxSeqSent = nextSeq > maxSeqSent ? nextSeq : maxSeqSent;
}

void Flow::print() {
	*debugSS << "Flow " << getName() << ". nextSeq " << nextSeq << " . sendBase " << sendBase << ". dupAcks " << dupAcks 
		<< ". CWND " << CWND << ". ssThresh " << ssThresh << ". gotAcks" << gotAcks
		<< ". estRTT" << estRTT << ". devRTT" << devRTT << ". sampRTT" << sampRTT << ". TO " << TO 
		<< std::endl;
}
