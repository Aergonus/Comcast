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
#include "packet.h"

class node;
class event;

class flow {
	friend class packet;
	
<<<<<<< HEAD
    private:
	bool gapDetected;
	
        node* src; //Packet source
        node* dst; //Packet destination
        std::string id; //Packet id
        int size;
        int nflow;
	float start;

	// TCP Parameters
	TCP_type mode;
	TCP *algo;
	// Reliable Data Transfer 
	// Sender
	int nextSeq, sendBase, dupAcks;
	std::map<int, int> ackStack; // Really AckVec or AckList, but it rhymes
	// Receiver
	int expectedSeq;
	
	// Congestion Control
	int CWND, ssThresh, gotAcks;
	
	// TimeOut Calculations
	int timedAck;
	float recordTime, estRTT, devRTT, sampRTT, TO;
	event *tcpTO;
	
	// Packet size calculation
	int calcPakSize(int);
	
	// Flow rate calculation
	// Time elapsed
	float time_elapsed;
	// Last update time
	float update_time;
	// Bytes sent
	float bytes_sent;
		
    public:
    //Constructors
    flow(std::string id, node *src, node *dst, int flowSize, float startTime, TCP_type tcp, ...)
=======
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
>>>>>>> 8b583dc3049d36ee176e40937169c7f1d9ee4fe5
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
	
    // obtain flowrate
    float flow_rate();
    // initiate flow
    void start_Flow();
    // flow timeout
    void flow_Timeout();
    // packet injection
    void send_packet(int, int, node*, packet_type);
    // packet received at host
    void receive_packet(packet*);
    // determine if flow ended
    bool noflow(){return nextSeq >= size;};
    // start sending all packets
    void send_All_Paks();
    void nolove();
	
};
#endif
