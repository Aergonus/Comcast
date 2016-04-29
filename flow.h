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
class event;
class Packet;

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
		
		// Packet size calculation
		int calcPakSize(int currSeq);
		
		// Flow rate calculation
		// Time elapsed
		float time_elapsed;
		// Last update time
		float update_time;
		// Bytes sent
		float bytes_sent;
		
    public:
    //Constructors
    Flow(std::string name, Node *src, Node *dst, int FlowSize, float startTime, TCP_type tcp)
		: name(name), src(src), dst(dst), size(FlowSize), start(startTime), mode(tcp){
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
	Packet* send_Pak(int pakNum, int pSize, Node *pakSrc, packet_type ptype);
	// Packet received at Host
	Packet* receive_Pak();
	// determine if Flow ended
	bool noFlow(){return nextSeq >= size;};
	void nolove();
	
	//debug and reporting
	std::string print();
};
#endif
