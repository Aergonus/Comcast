/**
 * ECE408 
 * link.h
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei
 * @version 0.1.5 04/05/16
 */

#ifndef LINK_H
#define LINK_H

#include <iostream>
#include <string>
#include <queue>
#include <utility>

#include "node.h"
#include "util.h"

// Forward Declaration
//class host;
class packet;
class net;

// External Variables 
extern bool debug;
extern ostream &debugSS;
extern ostream &errorSS;

class link {
	private:
		std::string name;
		// Node 1 and Node 2 endpoints of link
		node *n1, *n2;
		// Link Rate in bits per second (throughput)
		float rate;
		// Link Delay in seconds (propogation)
		float delay; 
		// Maximum number of bytes allowed
		int buffer_size;
		// Current amount of bytes in buffer
		int occupancy;
		// Link Buffer
		std::queue<std::pair<packet*,node*>> buffer;
		// Network Simulator 
		net Network;

	public:
		// CONSTRUCTOR
		link(std::string id, node *node1, node *node2, float rate, float delay, float buffer, net *sim)...
			:name(id), n1(node1), n2(node2), rate(rate), delay(delay), buffer_size(buffer), Network(sim){};
		
		// FUNCTIONS

		//Calculate delay for first packet in queue
		float calcDelay();
		
		// Pushes packet onto Link Buffer from end node; 
		// returns TRUE on success; FALSE on full buffer;
		// origin is a pointer to the packet's transmitting node;
		bool receive_pak(packet *p, node *n);
		
		// Called when packet has propogated and arrived at at node
		void send_pak();

		//DEBUG/LOGGING FUNCTIONS
		void print(){};
};
#endif