#include <vector>
#include <string>

#include "link.h"
#include "packet.h"
#include "host.h"

//FUNCTIONS
// Pushes packet onto Link Buffer from end node; 
// returns TRUE on success; FALSE on full buffer;
// origin is a pointer to the packet's transmitting node;
bool link::receive_pkt(packet* pkt, node* origin){
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
int link::transmit_pkt(){
	packet* pkt = link_buffer.front();
	(destination.front())->receive_packet(pkt);
	destination.pop();
	link_buffer.pop();
	return 1;
}

//Calculate delay for first packet in queue
float link::current_delay(){
			return (float)(link_buffer.front()->getSize())/throughput + propagation;
}

//Determines if link_buffer is empty
bool link::empty(){
	return link_buffer.empty();
}