#include "router.h"
#include "link.h"
#include "global.h"

Router::Router(const std::string id){
  id_ = id;
}

void Router::SendPacket(const Packet &p, const double time){
  links_[0].ReceivePacket(p, time, nodes_[0]);
}

void Router::RecievePacket(const Packet &p, const double time){
  cout<< "Packet " << p.id() << " received"<< " at time "<< time<<std::endl;
  received_packets_.push_back(p); 
  global::event_manager.push(SendPacketEvent( Packet("A", seqNum(), this, p.GetSrc()), time); 
} 

bool Router::allowedToTransmit(){
  return links_[0].isAvailable();
}

//separate vectors for router and host?
void UpdateTable(){
  UpdateCostVec();
  for router in routers{
    for routing_table[router_id]
      double temp = routing_table[router_id]->first() + cost_vector(router_id);
      if (temp < routing_table[this]) 
        routing_table[this] = temp;
  }
}

void UpdateCostVec(){ // updates cost vector every time step
  for l in link{
    cost_vector[i] = l.getCost(); // sum (packetSize*rate )
  }
}

Event SendControlEvent(control packet){
  //sends its distance vector to its neighbors
}

Event ReceiveControlEvent(control packet){
  routing_table[packet.getSrc().id()] = packet.getSrc().distanceVec ;
}

//initiate with greedy algorithm
