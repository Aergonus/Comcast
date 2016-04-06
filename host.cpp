/**
 * ECE408 
 * host.cpp
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei
 * @version 0.1.5 04/05/16
 */
 
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "link.h"
#include "host.h"

void host::addLink(link* l){
	links.push_back(l);
}

void host::addFlow(flow* f){
	flows.push_back(f);
}

int host::receive_packet(packet* p){
    std::vector<flow *>::iterator itr = std::find(flows.begin(), flows.end(), p->getFlow());
    assert(!itr == flows.end());
	(*itr)->receive_pak(p);
	return 0;
}