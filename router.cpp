#include "Router.h"
#include "events/event.h"
#include "events/event_manager.h"

Router::Router(std::string id): Node(id){}

void Router::receive_pak(Packet *p){
  if (p->type == CTRL){ //if the received Packet is control type
	receiveControl(p);
  }
  else{
	get_route(p.getDst()) -> receive_pak(p); //pushes the Packet to the Link
  }
} 

Link* Router::get_route(std::string Host_id){
  return next_hop_[Host_id];
}

//separate vectors for Router and Host?
void Router::update_table(std::string Router_id){
  routing_table[Router_id] = dynamic_cast<Router>(neighbors[Router_id]).routing_vector();
  updateCost();
}

//Implementation of Bellman-Ford
void Router::update_cost(){ // updates cost vector every time step
  for(auto &neighbor : Links){  
	//update the cost of each Link associated with different neighbor
	//neighbor.first = neighbor id
	//neighbor.second = Link pointer 
	costVec[neighbor.first] = neighbor.second -> Link_cost(); //Define Link Cost= sum (occupied buffer/rate) TODO: implement double Link_cost() function in Link.cpp
  }
  for(auto r : routing_table){ // r.first = Router id, r.second = map<Host id, cost> 
	for(auto &c : r.second){ //c.first = Host id, c.second = cost to the Host
	  double temp = c.second + costVec[r.first];			//if updated cost is less then the original cost, copy.
		if (temp < routing_table[Node::id_][c.first]){
		  routing_table[Node::id_][c.first] = temp;
		  next_hop[c.first] = r.first; //set next hop to be the Link with the minimum total cost
		}
	}
  }
}

//every 'x' time step, broadcast my routing table
void Router::send_control() {   
  for(auto &neighbor : neighbors){
	Packet p* = new Packet(this, neighbor.second, CTRL, 0); //TODO: killswitch default 0 okay?
	neighbor.second -> receive_pak(p);
  }
}

//whenever the Router receives ctrl Packet, update the routing table
void Router::receive_control(Packet* p){
  update_table(p->getSrc().id());
}

//returns the map<Host id, cost from "me" to the Host>
std::map<std::string, double> Router::routing_vector() {
  return routing_table[Node::id]; 
}


//initiate with greedy algorithm
