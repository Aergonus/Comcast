#include <iostream>
#include <string>
#include <queue>
#include "node.h"
#include "packet.h"


class Link {
	private:
		// throughput in bits per second & propagation in seconds
		float throughput, propagation; 
		// Maximum number of packets allowed
		int max_buffer_size;
		// Node 1 and Node 2 endpoints of link
		Node* n1,n2;
		// Link Buffer
		std::queue<packet*> link_buffer;
		std::queue<Node*> destination;

	public:
		// Pushes packet onto Link Buffer from end node; 
		// returns TRUE on success; FALSE on full buffer;
		// origin is a pointer to the packet's transmitting node;
		bool recieve_pkt(packet* pkt, Node* origin){
			if ((orig,in != n1)&&(origin != n2)){
				std::cerr <<"FATAL ERROR: NICE TRY. node is not connected to this link." << endl; 
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
		void transmit_pkt(packet& pkt){

		}
		//Calculate delay for first packet in queue
		float current_delay() {
			return (float) (link_buffer.front()->size())/throughput + propagation;
		}

		// CONSTRUCTOR
		Link(float t, float p, int b, Node* node1, Node* node2) {
			throughput = t; 
			propagation = p; 
			max_buffer_size = b;
			n1 = node1;
			n2 = node2;
		}
};