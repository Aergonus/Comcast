#ifndef LINK_H
#define LINK_H

#include <iostream>
#include <string>
#include <queue>
//#include "flow.h"
//#include "node.h"
#include "host.h"
//#include "packet.h"
#include "util.h"
//class host;
class packet;
class link {
	private:
		// throughput in bits per second & propagation in seconds
		float throughput, propagation; 
		// Maximum number of packets allowed
		int max_buffer_size;
		// Node 1 and Node 2 endpoints of link
		node* n1;
		node* n2;
		// Link Buffer
		std::queue<packet*> link_buffer;
		std::queue<node*> destination;

	public:
		// Pushes packet onto Link Buffer from end node; 
		// returns TRUE on success; FALSE on full buffer;
		// origin is a pointer to the packet's transmitting node;
		bool recieve_pkt(packet* pkt, node* origin){
			if ((origin != n1)&&(origin != n2)){
				std::cerr <<"FATAL ERROR: NICE TRY. node is not connected to this link." << std::endl; 
				return false;
			}
			if (link_buffer.size() < max_buffer_size){
				link_buffer.push(pkt);
				destination.push( (origin == n1) ? n2 : n1);
				return true;
			}	
			return false;
		}
		// Pops packet off Link Buffer and gives to end node
		int transmit_pkt(){
			packet* penis = link_buffer.front();
			(destination.front())->receive_packet(penis);
			destination.pop();
			link_buffer.pop();
			return 1;
		}
		//Calculate delay for first packet in queue
		float current_delay() {
			return (float) (link_buffer.front()->getSize())/throughput + propagation;
		}
		bool empty(){
			return link_buffer.empty();
		}
		// CONSTRUCTOR
		link(float t, float p, int b, node* node1, node* node2) {
			throughput = t; 
			propagation = p; 
			max_buffer_size = b;
			n1 = node1;
			n2 = node2;
		}
};

#endif