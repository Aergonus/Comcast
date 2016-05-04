/**
 * ECE408 
 * Router.cpp
 * Purpose: 
 * 
 * @author Eui Han, Arvind Nagalingam, Kangqiao Lei
 * @version 0.5.0 05/03/16
 */

#include <cfloat>

#include "Router.h"
#include "net.h"
#include "Host.h"
#include "Link.h"
#include "Packet.h"

void Router::receiveControl(Packet *p, Link *l){
	float pakCost = time - p->getCT();
	
	if (debug){
		*debugSS<<"ReceiveControl,"<<simtime<<","<<this->getName();
		for (const auto &z:updatedDVTime){
			*debugSS<<",m["<<z.first<<"] = "<<z.second;
		}
		*debugSS<<std::endl;
		*debugSS<<"ReceiveControl,"<<simtime<<","<<this->getName()<<"Pak";
		for (const auto &z:(*p->getuDVT())){
			*debugSS<<",m["<<z.first<<"] = "<<z.second;
		}
		*debugSS<<std::endl;
	}

	for(auto uDVT:(*p->getuDVT())){
		// Don't need to check for me DV since it won't ever be more updated, it'll just be ignored
		if(uDVT.first == this->getName()){
			continue;
		}
		auto itr = updatedDVTime.find(uDVT.first);
		if (itr != updatedDVTime.end()){ // If it is an immediate neighbor 

			if (debug){
				*debugSS<<"CheckDVT,"<<simtime<<","<<this->getName()<<uDVT.first<<",Pak,"<<uDVT.second<<",Have,"<<itr->second<<std::endl;
			}

			if (uDVT.second > itr->second){ // and the packet has newer cv
				itr->second = uDVT.second;

				if (debug){
					*debugSS<<"DVBefore,"<<simtime<<","<<getName();
					for (const auto &z:routing_table.at(uDVT.first)){
						*debugSS<<",m["<<z.first<<"] = "<<z.second;
					}
					*debugSS<<std::endl;
					*debugSS<<"DVPacket,"<<simtime<<","<<getName();
					for (auto &z:(*p->getRT()).at(uDVT.first)){
						*debugSS<<",m["<<z.first<<"] = "<<z.second;
					}
					*debugSS<<std::endl;
				}

				routing_table.at(uDVT.first) = (*p->getRT()).at(uDVT.first); // update rt

				if (debug){
					*debugSS<<"DVAfter,"<<simtime<<","<<getName();
					for (const auto &z:routing_table.at(uDVT.first)){
						*debugSS<<",m["<<z.first<<"] = "<<z.second;
					}
					*debugSS<<std::endl;
				}
			}
		}
	}
}

bool Router::updateCost(){

if (debug){
	*debugSS<<"UpdateCost,"<<simtime<<","<<getName()<<std::endl;
}

	// Only changes me's DV in table
	bool updated = false;
	// *errorSS<<"Updating CV for "<<getName()<<std::endl;
	// Update own cost vector
	for(auto &neighbor:getNeighbors()){
		// CostVector[myself] = 0
		if (neighbor.second->getName() == this->getName()){
			// *errorSS<<"Found myself!"<<std::endl;
			costVec.at(this->getName()) = 0; // getLink(this->getName())->get_cost();
		}
		// *errorSS<<"Checking "<<neighbor.first<<std::endl;
		// Get Link Cost for Routers only (Hosts guaranteed to be connected 0) 
		if (neighbor.second->isRouter){
		// *errorSS<<"Router "<<neighbor.second->getName()<<std::endl;
			costVec.at(neighbor.second->getName()) = getLink(neighbor.first)->get_cost();

if (debug){
	*debugSS<<"CVReport,"<<simtime<<","<<getName();
	for (const auto &p:costVec){
		*debugSS<<",m["<<p.first<<"] = "<<p.second;
	}
	*debugSS<<std::endl;
}

		} else {
		// *errorSS<<"Host "<<neighbor.second->getName()<<std::endl;
			// For our simulation Keep connected Hosts at cost 0. Assuming best connection to connected host
			// costVec.at(neighbor.second->getName()) = getLink(neighbor.second->getName())->get_cost();
		}
		// *errorSS<<"Processed "<<neighbor.second->getName()<<std::endl;
	}
	// *errorSS<<"Updating RT for "<<getName()<<std::endl;
	for(auto &r:routing_table){
		for(auto &c:r.second){
			// Don't need to calculate cost to itself
			if(r.first == this->getName()){
				// *errorSS<<"Found myself!"<<std::endl;
				continue;
			}
			// *errorSS<<"Looking at,"<<r.first<<",\t"<<c.first<<","<<routing_table.at(this->getName()).at(c.first)<<std::endl;
			float tCost = c.second + costVec.at(r.first);
			// *errorSS<<"tCost,"<<tCost<<",\t"<<c.first<<","<<routing_table.at(this->getName()).at(c.first)<<std::endl;
			if (tCost < routing_table.at(this->getName()).at(c.first)){
				routing_table.at(this->getName()).at(c.first) = tCost;
				// Get the Link name associated with the connected neighbor router
				std::string lid;
				for (auto &itr:getNeighbors()){
					if (r.first == itr.second->getName()){
						lid = itr.first;
						break;
					}
				}
				// *errorSS<<"NextHop,"<<c.first<<",lid,"<<lid<<std::endl;
				next_hop.at(c.first) = lid;
				updated = true;
			}
		}
	}
	// *errorSS<<"Finished RT for "<<getName()<<std::endl;
	updatedDVTime.at(this->getName()) = simtime;
	// Send updates of DVT 
	/*
	if(updated){
		sendCtrl();
	}
	*/
	return updated;
}

void Router::fwdPak(Packet *p, Link *fromLink){
	std::string hostId = p->getDst()->getName();
	auto itr = next_hop.find(hostId);
	
	Link* l;
	std::string min_link;
	if(itr->second == "DNE"){
		// No information initially, greedy search
		float min_cost = 0.99*FLT_MAX;
		for (auto checkLink:getLinks()){
			// Don't ping pong back packet
			if (checkLink->getName() == fromLink->getName())
				{continue;}
			// If connected to dst Host, send!
			if (!(getNeighbors().at(checkLink->getName())->isRouter)){
				if (getNeighbors().at(checkLink->getName())->getName() == hostId){
					checkLink->receive_pak(p,getNeighbors().at(checkLink->getName())); // Equivalent to getConnectedNode(checkLink);
					return;
				} else {
					continue;
				}
			}
			if (checkLink->get_cost() < min_cost){
				min_cost = checkLink->get_cost();
				min_link = checkLink->getName(); // if minlink doesn't get filled here 
			}
		}
		
		// Ok... fine let's play pong
		if (min_link == ""){
			l = fromLink;
		} else {
			l = getLink(min_link);
		}
	} else {
		l = getLink(next_hop.at(hostId));
	}
	l->receive_pak(p,this->getConnectedNode(l));
}

void Router::sendCtrl(){
	Packet *p;
	for (auto l:getLinks()){
		Node *rdst = this->getConnectedNode(l);
		if (rdst->isRouter){
			p = new crtl_pak(this, rdst, ROUTING_PACKET_SIZE, KS_POISION_CONSTANT, ROUTING, routing_table, updatedDVTime);

if (debug){
	*debugSS<<"CreatePak,"<<simtime<<",Created in Router sendCrtl,";
	p->print();
}

			// Injects the Packet into the Link
			l->receive_pak(p,rdst);
		}
	}
}

void Router::receive_pak(Packet *p, Link *l){
	// *errorSS<<"Received Pak"<<std::endl;
	if (p->type == ROUTING){
		// *errorSS<<"Received Rout Pak"<<std::endl;
		// Update DVT entries with received information
		receiveControl(p,l);
		// updateCost();
		// Doesn't need to update since updates don't sent until STALE_ROUTING_TIMEOUT
		// It will receive some packet before that and update its cost then (avoids unnecessary computing)
		// Buffers information for now;we do not propogate information to keep routing congestion low
	} else {
		// *errorSS<<"Received Data"<<std::endl;
		// Every time receive packet update own cost vector
		updateCost();
		// Hacky avoid loops 
		if(p->perishSong() > 0){
			fwdPak(p,l);
		} else {
			delete p;
		}
	}
	
	// Router is active! Check if it's time to send new control packets 
	if (simtime > ctrl_Updated + STALE_ROUTING_TIMEOUT){
		ctrl_Updated = simtime;
		sendCtrl();
	}
}

void Router::init(){

if (debug){
	*debugSS<<"InitRouter,"<<simtime<<","<<getName()<<",Start"<<std::endl;
}

	std::map<std::string, float> myDV;
	for (auto &hosts:Network->getHosts()){
		myDV.insert({hosts->getName(), 0.99*FLT_MAX}); // set distance to max for unconnected hosts, max*.99 to account for adding overflow
	}
	for(auto &links:getLinks()){
		Node *n = this->getConnectedNode(links);
		if(!(n->isRouter)){
			myDV[n->getName()] = 0; // set distance to 0 if connected to host
		}
	}
	
	routing_table.insert({this->getName(), myDV});
	
	std::map<std::string, float> innerDV;
	for (auto &nodes:getNeighbors()){ // for all neighboring nodes
		if (nodes.second->isRouter){ // Hosts do not have a routing table
			for (auto &hosts:Network->getHosts()){
				innerDV.insert({hosts->getName(), 0.99*FLT_MAX}); // set distance to max for routers, max*.99 to account for adding overflow
			}
			routing_table.insert({nodes.second->getName(), innerDV});
			updatedDVTime.insert({nodes.second->getName(), simtime});
		} else {
			continue;
		}
	}
	updatedDVTime.insert({this->getName(), simtime}); // Initialize my own updateDVTime
	
	for (auto &nodes:getNeighbors()){ // initialize the cost vector <neighboring router id, link cost>
		if (nodes.second->isRouter){ 
			costVec.insert({nodes.second->getName(), getLink(nodes.first)->get_cost()}); // initialize cost vector for neighbor
		} else {
			// if host the cost will be "0"
			continue;
		}
	}
	
	for (auto &hosts:Network->getHosts()){
		for(auto &links:getLinks()){
			if (hosts->getName() == getNeighbors().at(links->getName())->getName()){
				next_hop.insert({hosts->getName(), links->getName()});
			} else {
				next_hop.insert({hosts->getName(), "DNE"}); // Does Not Exist
			}
		}
	}
	updateCost();

if (debug){
	*debugSS<<"InitRouter,"<<simtime<<","<<getName()<<",End"<<std::endl;
}

}