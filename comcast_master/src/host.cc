#include <iostream>
#include "host.h"
#include "link.h"

extern EventManager event_manager;

Host::Host(const std::string id) : Node::Node(id){}

Event Host::SendPacket(const Packet &p, const double time){
  return SendPacketEvent( Packt("D", p.seqNum(), *this, p.GetSrc()), *this, link, time);
  links_[0].ReceivePacket(const_cast<Packet&>(p), time, nodes_[0]);
}

Event Host::RecievePacket(const Packet &p, const double time){
  std::cout<< "Packet " << p.id() << " received"<< " at time "<< time<<std::endl;
  received_packets_.push_back(p); 
  return SendPacketEvent( Packet("A", p.seqNum(), *this, p.GetSrc()), time); 
} 

const bool Host::allowedToTransmit() const{
  return links_[0].isAvailable();
}
