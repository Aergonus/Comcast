#ifndef LINK_H
#define LINK_H
 
#include <iostream>
#include <string>
#include <queue>

#include "node.h"
#include "util.h"

class host;
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
		// CONSTRUCTOR
		link(float t, float p, int b, node* node1, node* node2) {
			throughput = t; 
			propagation = p; 
			max_buffer_size = b;
			n1 = node1;
			n2 = node2;
		}

		//FUNCTIONS
		// Pushes packet onto Link Buffer from end node; 
		// returns TRUE on success; FALSE on full buffer;
		// origin is a pointer to the packet's transmitting node;
		bool receive_pkt(packet* pkt, node* origin);
		// Pops packet off Link Buffer and gives to end node
		int transmit_pkt();
		//Calculate delay for first packet in queue
		float current_delay();
		//Determines if link_buffer is empty
		bool empty();

		//DEBUG FUNCTIONS
		//prints out each packet and it's index
		int print_contents(){
			for(int i=0; i <= link_buffer.size(); i++) {
				std::cout<<"packet "<< i <<std::endl;
			}
			return 0;
		}
};
#endif