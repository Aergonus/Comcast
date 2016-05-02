/**
 * ECE408 
 * Router.cpp
 * Purpose: 
 * 
 * @author Eui Han, Arvind Nagalingam, Kangqiao Lei
 * @version 0.2.0 04/19/16
 */

#include <cfloat>

#include "Link.h"
#include "Router.h"

void Router::fwdPak(Packet *p, Link *fromLink){
	auto itr = next_hop.find(p->getDst()->getName()); // Host ID
	Link* l;
	if(itr->second == "DNE"){
		// No information initially, greedy search
		float min_cost = FLT_MAX;
		for( auto lid : getLinks()){
			if (lid->getName() == fromLink->getName())
				{continue;}
			if (neighbors.at(lid->getName())[0] == 'H'
			 || neighbors.at(lid->getName())[0] == 'T'
			 || neighbors.at(lid->getName())[0] == 'S'){
				if (neighbors.at(lid->getName()) == hid){
					l = lid;
				} else {
					continue;
				}
			}
			if (lid->GetCost() < min_cost){
				min_cost = lid->GetCost();
				min_link = lid->getName(); // if minlink doesn't get filled here 
			}
		}

		if (min_link == "") {
			l = fromLink;
		} else {
			l = getLink(min_Link);
		}
	} else {
		l = getLink(next_hop.at(hostId));
	}
	l->receive_pak(p,getConnectedNode(l);
}

void Router::receive_pak(Packet *p, Link *l){
	// Assumes flushed Links at time called 
	if (p->type == ROUTING){ //if the received Packet is control type
		receiveControl(p,l);
	} else {
		fwdPak(p);
	}
  if (t > ctrl_Updated + 20){
    ctrl_Updated = t;
    SendControl();
   }
}



//separate vectors for router and host?
void Router::UpdateTable(std::string router_id){
  routing_table_.at(router_id) = event_manager.Net().GetRouter(router_id).RoutingVector();
  UpdateCost();
}

//Implementation of Bellman-Ford
void Router::UpdateCost(){ // updates cost vector every time step
  bool updated = false;
  for(auto &neighbor : neighbors){
    if(neighbor.first[0] == 'R'){
      costVec.at(neighbor.second) = event_manager.Net().GetLink(neighbor.first).GetCost(); // sum (packetSize/rate)
    }
  }
  for(auto &r : routing_table_){
    for(auto &c : r.second){
      if(r.first == id_){continue;} //dont need to calculate the Cost to itself
      float temp = c.second + costVec.at(r.first);
      if (temp < routing_table_.at(id_).at(c.first)){ 
        routing_table_.at(id_).at(c.first) = temp;
        std::string lid;
        for (auto &itr : neighbors){
          if (r.first == itr.second){
            lid = itr.first;
            break;
          }
        }
        next_hop.at(c.first) = lid;
        updated = true;
     }
    }
  }
}

void Router::SendControl(){
  int i = 0;
  for(auto &link : links_){ // links_ --> getLinks()
    Link& l = event_manager.Net().GetLink(link);
    Node& n = event_manager.Net().GetNode(neighbors.at(link));
    event_manager.push(std::shared_ptr<Event>(new TransmitPacketEvent(l, n, Packet("FC", 'C', i, *this, n), event_manager.time())));
    ++i;
  }
}

void Router::receiveControl(Packet p){
  UpdateTable(p.GetSrc().id());
}

std::map<std::string, float> Router::RoutingVector() const{
  return routing_table_.at(id_);
}




void Router::Init(){
  std::map<std::string, float> inner;
  for (auto &itr : Network->getHosts()){
    for(auto &iitr : links_){
      if(neighbors.at(iitr) == itr.first){ // if i am connected to a host,
        inner.insert({itr.first, 0});    // set distance to 0
      }
      else{
        inner.insert({itr.first, FLT_MAX});  //set distance to every other host to million
      }
    }
  }

  routing_table_.insert({id_, inner});
  
  std::map<std::string, float> inner2;
  for (auto &itr : nodes_){ //for all my neighbor nodes
    if(itr[0] == 'H' || itr[0] == 'S' || itr[0] == 'T'){ // hosts do not have a routing table
      continue;
    }
    else{
      for(auto &h : event_manager.Net().GetHosts()){
        inner2.insert({h.first, FLT_MAX}); // make everything million
      }
      routing_table_.insert({itr, inner2});
    }
  }

  for (auto &neighbor : nodes_){ //initialize the cost vector <neighboring router id, link cost>
  //  std::cout<<"r_table: {"<<neighbor<<", inner}"<<std::endl;
    if(neighbor[0] =='R'){
      costVec.insert({neighbor, FLT_MAX});
    }
  }

  for(auto &itr : event_manager.Net().GetHosts()){
      for(auto &iitr : links_){
        if (itr.first == neighbors.at(iitr)){
          next_hop.insert({itr.first, iitr});
        }
        else{
          next_hop.insert({itr.first, "DNE"}); //Does Not Exist
        }
      }
  }
  UpdateCost();
}

// Gets the other Node connected to the Link
Node* Router::getConnectedNode(Link *connection){
	return connection->getOtherNode(this);
}

void Router::print() {

}