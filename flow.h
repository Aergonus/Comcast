/**
 * ECE408 
 * flow.h
 * Purpose: 
 * 
 * @author EuiSeong Han, Eric Nguyen, Kangqiao Lei, Jaeryung Song
 * @version 0.1.8 04/21/16
 */

#ifndef FLOW_H
#define FLOW_H

#include <set>
#include <cctype>
#include <string>
#include <algorithm>
#include "tcp.h"
#include "util.h"

class node;
class event;

struct comparePackets {
  bool operator() (std::pair<int,int> pakA, std::pair<int,int> pakB) {
	return pakA.first > pakB.first;
  }
};

class flow {
	friend class packet;
	
	private:
        node* src;
        node* dst;
        std::string name;
        int size;
		float start;
		
		// TCP Parameters
		TCP_type mode;
		TCP algo;
		// Reliable Data Transfer 
		// Sender
		int nextSeq, sendBase, dupAcks;
		std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, comparePackets> ackStack; // Really AckVec or AckList, but it rhymes
		// Receiver
		bool gapDetected;
		int maxGapSeq, expectedSeq; // Currently implemented Go-Back-N 
		
		// Fun project: Code Sliding Window selective-repeat
		// If smart application create dataGap with <expectedSeq, maxReceivedSeq> => recieve part of gap in middle! <expectedSeq, receivedSeq> <receivedSeq+size,maxRecceivedSeq>. 
		
		// Congestion Control
		int CWND, ssThresh, gotAcks;
		
		// TimeOut Calculations
		int timedAck;
		float recordTime, estRTT, devRTT, sampRTT, TO;
		event *tcpTO;
		
		int calcPakSize();
		
    public:
    //Constructors
    flow(std::string id, node src, node dst, int flowSize, float startTime, TCP_type tcp)...
	: name(id), src(src), dst(dst), size(flowSize), start(startTime) mode(tcp){
		if (mode == TAHOE) {
			algo = new TAHOE_TCP();
		} else if (mode == RENO) {
			algo = new RENO_TCP();
		}
	};

    //function to obtain the origin of the flow
    node* getSrc(){return src;};
    //function to obtain the destination of the flow
    node* getDst(){return dst;};
    //function to obtain the id of the flow
    string getID(){return id;};
    //function to obtain the size of the flow
    int getSize(){return size;};
	
	
	void start_Flow();
	void flow_Timeout();
	packet* send_Pak();
	packet* recieve_Pak();
	bool noflow(){return nextSeq >= size;};
	void nolove();
	
};
#endif