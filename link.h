/**
 * ECE408 
 * Link.h
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei
 * @version 0.2.0 04/19/16
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
		// Current amount of bytes in buffer
		int occupancy;
		// Link Buffer
		std::queue<std::pair<Packet*,Node*>> buffer;
		// Network Simulator 
		net *Network;
		
		// Link Flow rate calculation
		// Time elapsed
		float time_elapsed;
		// Last update time
		float update_time;
		// Bytes sent
		float bytes_sent;

	public:
		// CONSTRUCTOR
		Link(std::string id, Node *Node1, Node *Node2, float rate, float delay, float buffer, net *sim)
			:name(id), n1(Node1), n2(Node2), rate(rate), delay(delay), buffer_size(buffer), Network(sim){};
		
		// FUNCTIONS

		//Calculate delay for first Packet in queue
		float calcDelay();
		
		// Calculate Flowrate in Link
		float Link_Flow_rate();
		
		// Pushes Packet onto Link Buffer from end Node; 
		// returns TRUE on success; FALSE on full buffer;
		// origin is a pointer to the Packet's transmitting Node;
		bool receive_pak(Packet *p, Node *n);
		
		// Find Node on the other side of the Link
		Node* getOtherNode(Node *n);
		
		// Called when Packet has propogated and arrived at at Node
		void send_pak();

		//DEBUG/LOGGING FUNCTIONS
		std::string print();
};
#endif
