/**
 * ECE408 
 * Flow.cpp
 * Purpose: 
 * 
 * @author EuiSeong Han, Eric Nguyen, Kangqiao Lei, Jaeryung Song
 * @version 0.5.0 05/03/16
 */

#include <queue>
#include <math.h>
#include <limits.h>
#include <utility>

#include "Flow.h"
#include "net.h"
#include "Link.h"
#include "Packet.h"
#include "events/event_TO.h"

// Calculates the Packet size
// Has to deal with lack Packet being possibly smaller than the max_seg_size
int Flow::calcPakSize(int currSeq){
	int nPakSize = (size - currSeq <= MAX_SEG_SIZE) ? (size - currSeq) : MAX_SEG_SIZE;
#ifndef NDEBUG
if (debug) {
	*debugSS<<"CalcNextPak,"<<simtime<<",seqNum,"<<currSeq<<",ackNum,"<<nPakSize<<std::endl;
}
#endif
	return nPakSize;
}

// Injects the Packet into the system from the Flow
bool Flow::send_Pak(int pakNum, int pSize, Node *pakSrc, packet_type ptype){
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
#ifndef NDEBUG
if (debug) {
	*debugSS<<"CreatePak,"<<simtime<<",Created in Flow sendpak,";
	p->print();
}
#endif
	// Calls Link's Recieve Packet (p,nextNodeHop)
	return pakSrc->getLinks().front()->receive_pak(p, pakSrc->getConnectedNode(pakSrc->getLinks().front()));
}

// Sends Packets until the number of DATA Packets from this Flow in the system is equal to CWND
void Flow::send_All_Paks(){
	int nDroppedPaks = 0;
	while(((nextSeq - sendBase)/MAX_SEG_SIZE) < CWND && !noFlow()) {
		int pakSize = calcPakSize(nextSeq);
		if (!send_Pak(nextSeq, pakSize, dst, DATA)) {
			nDroppedPaks++;
		}
		// Increments the sequence number of the next Packet to be sent by the Packet size
		nextSeq += pakSize;
		// Timeout mechanism
		if (timedAck == -1 && nextSeq > maxSeqSent) {
			timedAck = nextSeq;
			recordTime = simtime;
#ifndef NDEBUG
if (debug) {
	*debugSS<<"TimedACK,"<<simtime<<",nSeqNum,"<<timedAck<<",recordTime,"<<recordTime<<std::endl;
}
#endif
		}
		maxSeqSent = nextSeq > maxSeqSent ? nextSeq : maxSeqSent;
	}
	//*outputSS<<simtime<<","<<getName()<<",nPakLoss,"<<nDroppedPaks<<std::endl;
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
	ssThresh = INT_MAX;
	
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
#ifndef NDEBUG
if (debug) {
	*debugSS<<"CreateEvent,"<<simtime<<",";
	tcpTO->print();
}
#endif
}

// Receive Packet event for Hosts
void Flow::receive_Pak(Packet *p){ 
	if(p->type == DATA){
	// Receiver receives the DATA
		// Would have to add 500ms timeout if waiting to send delayed acks
		logPakDelay(p->getCT());
#ifndef NDEBUG
if (debug) {
	*debugSS<<"GotData,"<<simtime<<",";
		p->print();
}
#endif
		// ACK generation 
		if (ackStack.empty()) {
#ifndef NDEBUG
if (debug) {
	*debugSS<<"EmptyAckStack,"<<simtime<<std::endl;
}
#endif
			ackStack.insert(std::make_pair(p->getSeqNum(),p->getAckNum())); 
			expectedSeq = p->getAckNum();
			send_Pak(expectedSeq, ACK_PACKET_SIZE, src, ACK);
		} else if(p->getSeqNum() == ackStack.begin()->second) {
#ifndef NDEBUG
if (debug) {
	*debugSS<<"Pushed AckStack,"<<simtime;
	for (const auto &p : ackStack) {
		*debugSS<<",ackStack["<<p.first<<"] = "<<p.second;
	}
	*debugSS<<std::endl;
}
#endif
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
#ifndef NDEBUG
if (debug) {
	*debugSS<<"Popped AckStack,"<<simtime;
	for (const auto &p : ackStack) {
		*debugSS<<",ackStack["<<p.first<<"] = "<<p.second;
	}
	*debugSS<<std::endl;
}
#endif
			send_Pak(expectedSeq, ACK_PACKET_SIZE, src, ACK);
			
		// Data gap detection
		} else if(p->getSeqNum() > ackStack.begin()->second) {
#ifndef NDEBUG
if (debug) {
	*debugSS<<"Gap detected!,"<<simtime;
	for (const auto &p : ackStack) {
		*debugSS<<",ackStack["<<p.first<<"] = "<<p.second;
	}
	*debugSS<<std::endl;
}
#endif
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
#ifndef NDEBUG
if (debug) {
	*debugSS<<"ExpectedAckReceived,"<<simtime<<",";
		p->print();
}
#endif
			// Record Flowrate upon receiving good ACK
			bytes_sent += p->getAckNum() - sendBase;
			//logFlowRate();

			sendBase = p->getAckNum();
			if (sendBase == size) {
				// Check if Flow finished
#ifndef NDEBUG
if (debug) {
	*debugSS<<"Processed,"<<simtime<<",";
				p->print();
	*debugSS<<"FlowFinished,"<<simtime<<getName()<<std::endl;
}
#endif
				Network->FlowFinished();
				//Network->vomitEvents();
				
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
				//logRTTO();
				
#ifndef NDEBUG
if (debug) {
	*debugSS<<"UpdateRTT,"<<simtime<<","<<getName()<<",sampRTT,"<<sampRTT<<",TO,"<<TO<<std::endl;
}
#endif
				timedAck = -1;
			}
			if (dupAcks > 3) {
				if (ssThresh != INT_MAX) {logSSThresh();}
				algo->fastRecovery(&CWND, &ssThresh);
				logCWND();
				logSSThresh();
				gotAcks = 1;
			} else if (CWND >= ssThresh) {
#ifndef NDEBUG
if (debug) {
	*debugSS<<"CongestAvoid,"<<simtime<<",CWND,"<<CWND<<",SSTHRESH,"<<ssThresh<<",GotACKS,"<<gotAcks<<std::endl;
}
#endif
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
#ifndef NDEBUG
if (debug) {
	*debugSS<<"CreateEvent,"<<simtime<<",";
			tcpTO->print();
}
#endif
		} else if (p->getAckNum() == sendBase) {
			// Received a duplicate ack, increase the counter
#ifndef NDEBUG
if (debug) {
	*debugSS<<"DupAck,"<<sendBase<<std::endl;
}
#endif
			dupAcks++;
			if(dupAcks == 3) {
			//TCP fast retransmit
				if (ssThresh != INT_MAX) {logSSThresh();}
				algo->fastRetransmit(&CWND, &ssThresh);
				logCWND();
				logSSThresh();
				
				int pakSize = calcPakSize(sendBase);
				send_Pak(sendBase, pakSize, dst, DATA);
				
				//logRTTO();
				tcpTO->invalidate();
				tcpTO = new event_TO(TO,this);
				Network->addEvent(tcpTO);
#ifndef NDEBUG
if (debug) {
	*debugSS<<"CreateEvent,"<<simtime<<",";
		tcpTO->print();
}
#endif
			} else if(dupAcks > 3) {
				// More than 3 dupAcks go into cwnd increase
				algo->tripDups(&CWND);
				//logCWND();
				//send_All_Paks(); // Shouldn't send anything
				
				// Move TO window to when packets are sent 
				//logRTTO();
				tcpTO->invalidate();
				tcpTO = new event_TO(TO,this);
				Network->addEvent(tcpTO);
#ifndef NDEBUG
if (debug) {
	*debugSS<<"CreateEvent,"<<simtime<<",";
		tcpTO->print();
}
#endif
			}
			
		// Record out of order acks
		} else if (p->getAckNum() < sendBase) {
#ifndef NDEBUG
if (debug) {
	*debugSS<<"OUT OF ORDER "<<p->getAckNum()<<std::endl; 
}
#endif
		}
	}
	// Packet no longer useful, eliminate
#ifndef NDEBUG
if (debug) {
	*debugSS<<"Processed,"<<simtime<<",";
	p->print();
}
#endif
	delete p; 
}

// Timeout occurs
void Flow::timeout_Flow() {
	// Debug for timeout events
#ifndef NDEBUG
if (debug) {
	*debugSS<<"Timeout,"<<simtime<<",AckNumTO,"<<(sendBase + calcPakSize(sendBase))<<",oldCWND,"<<CWND<<",oldSSThresh,"<<ssThresh<<std::endl;
}
#endif
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
#ifndef NDEBUG
if (debug) {
	*debugSS<<"CreateEvent,"<<simtime<<",";
		tcpTO->print();
}
#endif
	int pakSize = calcPakSize(sendBase);
	send_Pak(sendBase, pakSize, dst, DATA);
	nextSeq = sendBase + pakSize;
	maxSeqSent = nextSeq > maxSeqSent ? nextSeq : maxSeqSent;
}

void Flow::print() {
#ifndef NDEBUG
if (debug) {
	*debugSS<<"Flow "<<getName()<<". nextSeq "<<nextSeq<<" . sendBase "<<sendBase<<". dupAcks "<<dupAcks 
		<<". CWND "<<CWND<<". ssThresh "<<ssThresh<<". gotAcks"<<gotAcks
		<<". estRTT"<<estRTT<<". devRTT"<<devRTT<<". sampRTT"<<sampRTT<<". TO "<<TO 
		<<std::endl;
}
#endif
}