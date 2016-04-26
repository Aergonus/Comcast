#include "link.h"
#include "node.h"
#include <vector>
#include <cmath>

Link::Link(const std::string id, const Node& end1, const Node& end2, 
           double datarate, double buffer_size, double delay):
  id_(id),
  datarate_(datarate),
  buffer_size_(buffer_size),
  delay_(delay + global::PACKET_SIZE / datarate){

  if (global::PACKET_SIZE % datarate !=0) {++delay;}
  end1_ = end1;
  end2_ = end2;
  buffer_ = Packet[buffer_size];
}

bool Link::isAvailable() const{
  return buffer_size_ == num_packs_in_buffer_;
}

void Link::ReceivePacket(Packet &p, double t, Node& to){
  if (transmitting) {
    buffer_[num_packs_in_buffer_] = p;
    ++num_packs_in_buffer_;
  }
  else{
    SendPacket(p, t + delay, to);
    transmitting = true;
  }
}

void Link::SendPacket(Packet &p, double t, Node& to){
  to.ReceivePacket(p,t);
}

void Link::DoneTransmitting(){
  transmitting = false;
}

std::string id() const{
  return id_;
}

double rate() const{
  return datarate_;
}

double LinkCost() const{
  return std::log10(1/(buffer_size_ - used_buffer_ + datarate));
}
