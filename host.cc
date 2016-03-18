#include <vector>
#include <string>
#include <iostream>

#include "link.h"
#include "host.h"

//FUNCTIONS
//add link pointer in link vector
void host::addLink(link* l){
	links.push_back(l);
}

//add flow pointer in flows vector
void host::addFlow(flow* f){
	flows.push_back(f);
}

//sends a packet into the network via the host's link
bool host::sendPak(){
	for(int i=0; i < flows[0]->numPack(); i++ ){
		if (! links[0]->receive_pkt(flows[0]->getPacket(i), this)){
			return false;
		}
	}
	return true;
}

//receives packet fro a Link
int host::receive_packet(packet* p){
	std::cout<< "Packet number "<<p->getSeqNum()<<" received from "<< p->getOrigin()->getName()<<std::endl;
	return 1;
}