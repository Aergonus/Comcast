/**
 * ECE408 
 * flow.cpp
 * Purpose: 
 * 
 * @author EuiSeong Han, Eric Nguyen, Kangqiao Lei
 * @version 0.1.5 04/05/16
 */

#include "flow.h"
#include "packet.h"

void start_Flow(){

	// Poke TCP and send event 
	// Start timeout for TCP 
	// Increment flows active 
	// Generates first send_pak event 
/* TODODODODODODODODODODODODO ****!!!*!*!*!*!*!*!*
		event aksjdflakjsld;fj 
		Network.addEvent(...);
*/
	
}

packet* send_Pak(int nSeqNum, int nAckNum, int, pSize, node *n){
	packet *p = new packet(src, dst, &this, DATA, pSize, nSeqNum, nAckNum, KS_POISION_CONSTANT); // gotta have correct #s
	link->receive_pak(p, n);
	return p;
}

void send_Data(){
	int pSize = left > DATA_PACKET_SIZE ? DATA_PACKET_SIZE : left;
	
	// calculate nSeqNum and nAckNum
	
	send_Pak(nSeqNum, nAckNum, pSize, src);
};

void send_Ack(packet *p){
	// calculate nSeqNum and nAckNum
	
	send_Pak(nSeqNum, nAckNum, pSize, dst);
};

void receive_Pak(packet *p){
	if(p->type == DATA){
		// Would have to add 500ms timeout if waiting to send delayed acks
		// Make ack_packet
		// Send packet
	} else if (p->type == ACK) {
		// Tell TCP that ack received
	} 
}


// If loss, set ssThresh to CWND/2
// TCP
// If CWND >= ssThresh, then no longer slow start but congestion avoidance
// Increment cwnd for ack, and check to send packet if not ignoring
// 