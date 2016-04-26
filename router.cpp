#include "router.h"
#include "event.h"
#include "event_manager.h"

router::router(std::string id): Node(id){}

void router::receive_pak(Packet *p){
  if (p->type == CTRL){ //if the received packet is control type
    receiveControl(p);
  }
  else{
    get_route(p.getDst()) -> receive_pak(p); //pushes the packet to the link
  }
} 

link* router::get_route(std::string host_id){
  return next_hop_[host_id];
}

//separate vectors for router and host?
void router::update_table(std::string router_id){
  routing_table[router_id] = dynamic_cast<router>(neighbors[router_id]).routing_vector();
  updateCost();
}

//Implementation of Bellman-Ford
void router::update_cost(){ // updates cost vector every time step
  for(auto &neighbor : links){  
    //update the cost of each link associated with different neighbor
    //neighbor.first = neighbor id
    //neighbor.second = link pointer 
    costVec[neighbor.first] = neighbor.second -> link_cost(); //Define Link Cost= sum (occupied buffer/rate) TODO: implement double link_cost() function in link.cpp
  }
  for(auto r : routing_table){ // r.first = router id, r.second = map<host id, cost> 
    for(auto &c : r.second){ //c.first = host id, c.second = cost to the host
      double temp = c.second + costVec[r.first];            //if updated cost is less then the original cost, copy.
        if (temp < routing_table[Node::id_][c.first]){
          routing_table[Node::id_][c.first] = temp;
          next_hop[c.first] = r.first; //set next hop to be the link with the minimum total cost
        }
    }
  }
}

//every 'x' time step, broadcast my routing table
void router::send_control() {   
  for(auto &neighbor : neighbors){
    packet p* = new packet(this, neighbor.second, CTRL, 0); //TODO: killswitch default 0 okay?
    neighbor.second -> receive_pak(p);
  }
}

//whenever the router receives ctrl packet, update the routing table
void router::receive_control(packet* p){
  update_table(p->getSrc().id());
}

//returns the map<host id, cost from "me" to the host>
std::map<std::string, double> router::routing_vector() {
  return routing_table[node::id]; 
}


//initiate with greedy algorithm
