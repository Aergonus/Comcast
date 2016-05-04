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

#include "net.h"
#include "Flow.h"
#include "Link.h"
#include "Packet.h"
#include "events/event_TO.h"

// Calculates the Packet size
// Has to deal with lack Packet being possibly smaller than the max_seg_size
int Flow::calcPakSize(int currSeq){
	int nPakSize = (size - currSeq <= MAX_SEG_SIZE) ? (size - currSeq):MAX_SEG_SIZE;
	if (debug){
		*debugSS<<"CalcNextPak,"<<simtime<<",seqNum,"<<currSeq<<",ackNum,"<<nPakSize<<std::endl;
	}
	return nPakSize;
}

// Injects the Packet into the system from the Flow
bool Flow::send_Pak(int pakNum, int pSize, Node *pakSrc, packet_type ptype){
	Packet *p;
	// Reverses direction of incoming Packet for the newly generated Packet
	Node *pakDst = (pakSrc == src) ? dst:src;
	// Packet Generation depending on type
	if (ptype == DATA){
		p = new data_pak(pakSrc, pakDst, pSize, KS_POISION_CONSTANT, ptype, pakNum, this);
	} else if (ptype == ACK){
		p = new ack_pak(pakSrc, pakDst, pSize, KS_POISION_CONSTANT, ptype, pakNum, this);
	}
	
	if (debug){
		*debugSS<<"CreatePak,"<<simtime<<",Created in Flow sendpak,";
		p->print();
	}
	
	// Injects the Packet into the Link by calling Link's Recieve Packet (p,nextNodeHop)
	return pakSrc->getLinks().front()->receive_pak(p, pakSrc->getConnectedNode(pakSrc->getLinks().front()));
}

// Sends Packets until the number of DATA Packets from this Flow in the system is equal to CWND
void Flow::send_All_Paks(){
	int nDroppedPaks = 0;
	while(((nextSeq - sendBase)/MAX_SEG_SIZE) < CWND && !noFlow()){
		int pakSize = calcPakSize(nextSeq);
		if (!send_Pak(nextSeq, pakSize, dst, DATA)){
			nDroppedPaks++;
		}
		// Increments the sequence number of the next Packet to be sent by the Packet size
		nextSeq += pakSize;
		// Timeout mechanism
		if (timedAck == -1 && nextSeq > maxSeqSent){
			timedAck = nextSeq;
			recordTime = simtime;
		
		if (debug){
			*debugSS<<"TimedACK,"<<simtime<<",nSeqNum,"<<timedAck<<",recordTime,"<<recordTime<<std::endl;
		}
		
		}
		maxSeqSent = nextSeq > maxSeqSent ? nextSeq:maxSeqSent;
	}
	// *outputSS<<simtime<<","<<getName()<<",nPakLoss,"<<nDroppedPaks<<std::endl;
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
	nextSeq += pakSize; // determines next DATA to be sent
	maxSeqSent = nextSeq > maxSeqSent ? nextSeq:maxSeqSent;
	
	// Timeout Initializations
	timedAck = nextSeq;
	recordTime = simtime;
	
	// Create Timeout Event
	tcpTO = new event_TO(TO,this);
	Network->addEvent(tcpTO);
}

// Receive Packet event for Hosts
void Flow::receive_Pak(Packet *p){ 
	if(p->type == DATA){
	// Receiver receives the DATA
		// Would have to add 500ms timeout if waiting to send delayed acks
		logPakDelay(p->getCT());
		
		if (debug){
			*debugSS<<"GotData,"<<simtime<<",";
				p->print();
		}
		
		// Keep track of average travel time for packets
		pakRTT = (pakRTT*paksReceived + simtime - p->getCT())/(paksReceived+1);
		paksReceived++;
		
		// ACK generation 
		if (ackStack.empty()){
			// Upon receiving first Data Packet
			if (debug){
				*debugSS<<"EmptyAckStack,"<<simtime<<std::endl;
			}
			ackStack.insert(std::make_pair(p->getSeqNum(),p->getAckNum()));
			
			expectedSeq = p->getAckNum();
			send_Pak(expectedSeq, ACK_PACKET_SIZE, src, ACK);
		} else if(p->getSeqNum() == ackStack.begin()->second){
			// Received next expected sequence number
			if (debug){
				*debugSS<<"Initial-AckStack,"<<simtime;
				for (const auto &p:ackStack){
					*debugSS<<",ackStack["<<p.first<<"] = "<<p.second;
				}
				*debugSS<<std::endl;
			}
			ackStack.erase(ackStack.begin()); // "Pop" low data tracker
			ackStack[p->getSeqNum()] = p->getAckNum(); // "Push" most recent data packet
			
			// Check if have buffered data packets to send back bigger ack if valid
			std::map<int,int>::iterator it;
			it = ackStack.find(p->getSeqNum());
			while (ackStack.find(it->second) != ackStack.end()){
				int nAck = it->second;
				ackStack.erase(it);
				it = ackStack.find(nAck);
			}
			expectedSeq = it->second; // Ack back highest contiguous data num 
			
			if (debug){
				*debugSS<<"Updated-AckStack,"<<simtime;
				for (const auto &p:ackStack){
					*debugSS<<",ackStack["<<p.first<<"] = "<<p.second;
				}
				*debugSS<<std::endl;
			}
			dupWait = -1; // Reset waiting interval
			send_Pak(expectedSeq, ACK_PACKET_SIZE, src, ACK);
		} else if(p->getSeqNum() > ackStack.begin()->second){
			// Data gap detection
			if (debug){
				*debugSS<<"Gap detected!,"<<simtime;
				for (const auto &p:ackStack){
					*debugSS<<",ackStack["<<p.first<<"] = "<<p.second;
				}
				*debugSS<<std::endl;
			}
			
			// Buffer Out of Seq Data
			ackStack.insert(std::make_pair(p->getSeqNum(),p->getAckNum()));
			
			// Send duplicate ack
			//if (simtime > dupWait) {
			//	dupWait = simtime + pakRTT;
				send_Pak(expectedSeq, ACK_PACKET_SIZE, src, ACK);
			//}
		} else {
			// Got something lower, send back expected ack // or ignore?
			send_Pak(expectedSeq, ACK_PACKET_SIZE, src, ACK);
		}
	} else if (p->type == ACK){
	// Transmitter receives an ACK
		if(p->getAckNum() > sendBase){
			// Receive "expected" ACK number (sendbase + 1)
			if (debug){
				*debugSS<<"ExpectedAckReceived,"<<simtime<<",";
					p->print();
			}
			
			// Increment data received upon receiving good ACK (for logging Flow Rate)
			bytes_sent += p->getAckNum() - sendBase;
			sendBase = p->getAckNum();
			
			if (sendBase == size){
				// Check if flow finished and exectue cleanup and exit
				if (debug){
					*debugSS<<"Processed,"<<simtime<<",";
								p->print();
					*debugSS<<"FlowFinished,"<<simtime<<getName()<<std::endl;
				}
				
				// Logging at end of flow
				logRTTO();
				logCWND();
				logSSThresh();
				
				// Update Network Simulator (which can end when all flows finish)
				Network->FlowFinished();
				
				// Packet no longer useful, eliminate
				delete p;
				return;
			}
			
			if (sendBase >= timedAck && timedAck != -1){
				// RTT estimation and starting the TO timer
				sampRTT = simtime - recordTime;
				estRTT = (1-ALPHA_TIMEOUT) * estRTT + ALPHA_TIMEOUT * sampRTT;
				devRTT = (1-BETA_TIMEOUT) * devRTT + BETA_TIMEOUT * abs(sampRTT - estRTT);
				TO = estRTT + 4 * devRTT + 0.100; // Add 100 ms to avoid converge
				if (debug){
					logRTTO();
					*debugSS<<"UpdateRTT,"<<simtime<<","<<getName()<<",sampRTT,"<<sampRTT<<",TO,"<<TO<<std::endl;
				}
				timedAck = -1;
			}
			
			if (dupAcks > 3){
				// Process first non duplicate ack after dups
				if (ssThresh != INT_MAX){logSSThresh();}
				algo->fastRecovery(&CWND, &ssThresh);
				logCWND();
				logSSThresh();
				gotAcks = 1;
			} else if (CWND >= ssThresh){
				if (debug){
					*debugSS<<"CongestAvoid,"<<simtime<<",CWND,"<<CWND<<",SSTHRESH,"<<ssThresh<<",GotACKS,"<<gotAcks<<std::endl;
				}
				
				// Max Probing/Congestion avoidance
				if (gotAcks == CWND){// (sendBase == nextSeq - gotAcks*MAX_SEG_SIZE){
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
			
			dupAcks = 0;
			send_All_Paks();
			// Resets timer when an ACK is received
			tcpTO->invalidate();
			tcpTO = new event_TO(TO,this);
			Network->addEvent(tcpTO);
		} else if (p->getAckNum() == sendBase){
			// Received a duplicate ack, increase the counter
			if (debug){
				*debugSS<<"DupAck,"<<simtime<<","<<sendBase<<std::endl;
			}

			dupAcks++;
			if(dupAcks == 3){
			// TCP fast retransmit
				if (ssThresh != INT_MAX){logSSThresh();}
				algo->fastRetransmit(&CWND, &ssThresh);
				logCWND();
				logSSThresh();
				
				int pakSize = calcPakSize(sendBase);
				send_Pak(sendBase, pakSize, dst, DATA);
				
				// logRTTO();
				tcpTO->invalidate();
				tcpTO = new event_TO(TO,this);
				Network->addEvent(tcpTO);
			} else if(dupAcks > 3){
				// More than 3 dupAcks go into cwnd increase
				algo->tripDups(&CWND);
				logCWND();
				// send_All_Paks(); // Shouldn't send anything
				
				// Move TO window to when packets are sent 
				tcpTO->invalidate();
				tcpTO = new event_TO(TO,this);
				Network->addEvent(tcpTO);
			}
			
		// Record out of order acks
		} else if (p->getAckNum() < sendBase){
			if (debug){
				*debugSS<<"OutOfOrder,"<<simtime<<","<<p->getAckNum()<<std::endl;
			}
		}
	}
	// Packet no longer useful, eliminate
	if (debug){
		*debugSS<<"Processed,"<<simtime<<",";
		p->print();
	}
	delete p;
}

void Flow::timeout_Flow(){
	if (debug){
		*debugSS<<"Timeout,"<<simtime<<",AckNumTO,"<<(sendBase + calcPakSize(sendBase))<<",oldCWND,"<<CWND<<",oldSSThresh,"<<ssThresh<<std::endl;
	}
	
	if (ssThresh != INT_MAX){logSSThresh();} // Don't log undefined ssThresh initially
	ssThresh = (CWND / 2) < 2 ? 2:(CWND / 2);
	CWND = 1;
	logCWND();
	logSSThresh();
	
	// dupAcks = 0;
	timedAck = -1;
	estRTT *= 2; // Technically should have upper bound
	TO = estRTT + 4 * devRTT + + 0.100; // Add 100 ms to avoid converge
	logRTTO();
	
	tcpTO->invalidate();
	tcpTO = new event_TO(TO,this);
	Network->addEvent(tcpTO);

	int pakSize = calcPakSize(sendBase);
	send_Pak(sendBase, pakSize, dst, DATA);
	nextSeq = sendBase + pakSize;
	maxSeqSent = nextSeq > maxSeqSent ? nextSeq:maxSeqSent;
}

void Flow::print(){
	if (debug){
		*debugSS<<"Flow "<<getName()<<". nextSeq "<<nextSeq<<" . sendBase "<<sendBase<<". dupAcks "<<dupAcks 
			<<". CWND "<<CWND<<". ssThresh "<<ssThresh<<". gotAcks"<<gotAcks
			<<". estRTT"<<estRTT<<". devRTT"<<devRTT<<". sampRTT"<<sampRTT<<". TO "<<TO 
			<<std::endl;
	}
}