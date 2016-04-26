/**
 * ECE408 
 * flow.h
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

class node;
class event;

class flow {
	friend class packet;
	
	private:
        node* src;
        node* dst;
        std::string id;
        int size;
		float start;
		
		// TCP Parameters
		TCP_type mode;
		TCP *algo;
		// Reliable Data Transfer 
		// Sender
		int nextSeq, sendBase, dupAcks;
		std::map<int,int> ackStack; // Really AckVec or AckList, but it rhymes
		// Receiver
		int expectedSeq;
		
		// Congestion Control
		int CWND, ssThresh, gotAcks;
		
		// TimeOut Calculations
		int timedAck;
		float recordTime, estRTT, devRTT, sampRTT, TO;
		event *tcpTO;
		
		int calcPakSize();
		
    public:
    //Constructors
    flow(std::string id, node *src, node *dst, int flowSize, float startTime, TCP_type tcp)
	: id(id), src(src), dst(dst), size(flowSize), start(startTime), mode(tcp){
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
    std::string getID(){return id;};
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
