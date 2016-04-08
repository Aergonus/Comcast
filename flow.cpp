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

packet* send_Pak(){
	int pSize = left > DATA_PACKET_SIZE ? DATA_PACKET_SIZE : left;
	
	packet *p = new packet(src, dst, &this, DATA, pSize, nSeqNum, nAckNum, KS_POISION_CONSTANT); // gotta have correct #s
	
}
