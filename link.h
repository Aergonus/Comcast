/**
 * ECE408 
 * Link.h
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei
 * @version 0.5.0 05/03/16
 */

#ifndef LINK_H
#define LINK_H

#include <iostream>
#include <string>
#include <queue>
#include <utility>

#include "util.h"

// Forward Declaration
class Node;
//class Host;
class Packet;
class net;

class Link {
	private:
		std::string name;
		// Node 1 and Node 2 endpoints of Link
		Node *n1, *n2;
		// Link Rate in bits per second (throughput)
		float rate;
		// Link Delay in seconds (propogation)
		float delay;
		// Maximum number of bytes allowed
		int buffer_size;
		// Link Buffer
		std::queue<std::pair<Packet*,Node*> > buffer;
		// Network Simulator 
		net *Network;
		// Current amount of bytes in buffer
		int occupancy;
		int nAckPaks;
		int nDataPaks;
		int nBuffPaks;
		int nDroppedPaks;
		
		bool direction;
		int nFlips;
		
		// Link Flow rate calculation
		// Last update time
		float update_time;
		// Bytes sent
		float bytes_sent;

	public:
		// CONSTRUCTOR
		Link(std::string id) :name(id) {};
		Link(std::string id, Node *Node1, Node *Node2, float rate, float delay, float buffer, net *sim)
			:name(id), n1(Node1), n2(Node2), rate(rate), delay(delay), buffer_size(buffer), Network(sim){
			direction = true; // Sending Data
			};
		
		// FUNCTIONS

		//Calculate delay for first Packet in queue
		float calcDelay();
		
		// Calculate Flowrate in Link
		float get_link_flow_rate();
		
		// This is weirdly weighted, should use number of packets //maybe? // Since we changed prop delay this should be right
		float get_cost() {return (occupancy / rate) + delay;};
		
		// Get name for Link
		std::string getName(){return name;};
		
		// Pushes Packet onto Link Buffer from end Node; 
		// returns TRUE on success; FALSE on full buffer;
		// origin is a pointer to the Packet's transmitting Node;
		bool receive_pak(Packet *p, Node *n);
		
		// Find Node on the other side of the Link
		Node* getOtherNode(Node *n);
		
		// Called when Packet has propogated and arrived at at Node
		void send_pak();

		bool operator== (Link *cmpLink){
			return (this->getName() == cmpLink->getName());
		};
		//DEBUG/LOGGING FUNCTIONS
		void print();
		
		void debugBuffer();
		
		void logBuffer(){
#ifndef NDEBUG
if (debug) {
// Debug is wack for now, disabled so output isn't as long
/*
			float buffOcc_Percent = buffer_size == 0 ? 0 : (float) occupancy/buffer_size;
			float AckPak_Percent = nBuffPaks == 0 ? 0 : (float) nAckPaks/nBuffPaks;
			float DataPak_Percent = nBuffPaks == 0 ? 0 : (float) nDataPaks/nBuffPaks;
			*outputSS<<simtime<<","<<getName()<<","<<getName()<<"-BuffOcc%,"<<buffOcc_Percent<<std::endl;
			*outputSS<<simtime<<","<<getName()<<","<<getName()<<"-AckPak%,"<<AckPak_Percent<<std::endl;
			*outputSS<<simtime<<","<<getName()<<","<<getName()<<"-DataPak%,"<<DataPak_Percent<<std::endl;
*/
}
#endif
			*outputSS<<simtime<<","<<getName()<<","<<getName()<<"-BuffOcc,"<<occupancy<<std::endl;
		}
		
		void logDrops(){
			*outputSS<<simtime<<","<<getName()<<","<<getName()<<"-nDroppedPaks,"<<nDroppedPaks<<std::endl;
		};
		
		void logLinkRate(){
		// Calculates the Link rate for logging
			// Time elapsed since last update
			float time_elapsed = simtime - update_time;
			// Link rate is bytes sent over elapsed time (s)
			float l_rate = bytes_sent/time_elapsed;
			// Reset the bytes sent and most recent update time
			bytes_sent = 0;
			update_time = simtime;
			// Log Format 
			*outputSS<<simtime<<","<<getName()<<","<<getName()<<"-LinkRate,"<<l_rate<<std::endl;
		}

};
#endif