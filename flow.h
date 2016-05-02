/**
 * ECE408 
 * Flow.h
 * Purpose: 
 * 
 * @author EuiSeong Han, Eric Nguyen, Kangqiao Lei, Jaeryung Song
 * @version 0.2.0 04/21/16
 */

#ifndef FLOW_H
#define FLOW_H

#include <map>
#include <cctype>
#include <string>
#include <algorithm>
#include "tcp.h"
#include "util.h"

class Node;
class event_TO;
class Packet;
class net;

class Flow {
	//friend class Packet;
	
	private:
		std::string name; //Packet name
		Node* src; //Packet source
		Node* dst; //Packet destination
		int size;
		float start;

		// TCP Parameters
		TCP_type mode;
		TCP *algo;
		
		// Network Simulator 
		net *Network;
		
		// Reliable Data Transfer 
		// Sender
		int nextSeq, sendBase, dupAcks;
		std::map<int,int> ackStack; // Really AckVec or AckList, but it rhymes
		// Receiver
		int expectedSeq;
		
		// Congestion Control
		int CWND, ssThresh, gotAcks;
		
		// TimeOut Calculations
		int timedAck, maxSeqSent;
		float recordTime, estRTT, devRTT, sampRTT, TO;
		event_TO *tcpTO;
		
		// Packet size calculation
		int calcPakSize(int currSeq);
		
		// Flow rate calculation
		// Last update time
		float update_time;
		// Bytes sent
		float bytes_sent;
		
	public:
	//Constructors
	Flow(std::string name) : name(name) {};
	Flow(std::string name, Node *src, Node *dst, int FlowSize, float startTime, TCP_type tcp, net *sim)
		: name(name), src(src), dst(dst), size(FlowSize), start(startTime), mode(tcp), Network(sim) {
		update_time = startTime;
		if (mode == TAHOE) {
			algo = new TAHOE_TCP();
		} else if (mode == RENO) {
			algo = new RENO_TCP();
		}
	};

	//function to obtain the origin of the Flow
	Node* getSrc(){return src;};
	//function to obtain the destination of the Flow
	Node* getDst(){return dst;};
	//function to obtain the name of the Flow
	std::string getName(){return name;};
	//function to obtain the size of the Flow
	int getSize(){return size;};
	
	// obtain Flowrate
	float get_flow_rate();
	// initiate Flow
	void start_Flow();
	// Flow timeout
	void timeout_Flow();
	// Packet injection
	void send_All_Paks();
	bool send_Pak(int pakNum, int pSize, Node *pakSrc, packet_type ptype);
	// Packet received at Host
	void receive_Pak(Packet *p);
	// determine if Flow ended
	bool noFlow(){return nextSeq >= size;};
	void nolove();
	
	bool operator == (Flow *cmpFlow){
		return (this->getName() == cmpFlow->getName());
	};
	
	//debug and reporting
	void print();
	
	void logCWND() {
		*outputSS << simtime << "," << getName() << ",CWND," << CWND << std::endl;
	}
	void logSSThresh() {
#ifndef NDEBUG
if (debug) {
	*outputSS << simtime << "," << getName() << ",ssThresh," << ssThresh << std::endl;
}
#endif
	}
	void logRTTO() {
		*outputSS << simtime << "," << getName() << ",sampRTT," << sampRTT << std::endl;
#ifndef NDEBUG
if (debug) {
		*outputSS << simtime << "," << getName() << ",estRTT," << estRTT << std::endl;
		*outputSS << simtime << "," << getName() << ",devRTT," << devRTT << std::endl;
}
#endif
		*outputSS << simtime << "," << getName() << ",TO," << TO << std::endl;
	}
	
	void logFlowRate(){
	// Calculates the Flow rate for logging
		// Time elapsed since last update
		float time_elapsed = simtime - update_time;
		// Flow rate is bytes sent over elapsed time (s)
		float f_rate = bytes_sent/time_elapsed;
		// Reset the bytes sent and most recent update time
		bytes_sent = 0;
		update_time = simtime;
		// Log Format 
		*outputSS << simtime << "," << getName() << ",FlowRate," << f_rate << std::endl;
	}
};
#endif
