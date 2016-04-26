#ifndef LINK_H_
#define LINK_H_

class Node;
#include "packet.h"

class Link{
  public:
    Link(std::string, Node&, Node&, double, double, double);
    bool isAvailable() const;
    void ReceivePacket(Packet&, double, Node&);
    void SendPacket(Packet&, double, Node&);
    void DoneTransmitting();
    std::string id() const;
    double rate() const;
    double LinkCost() const;
  private:
    //constants
    const std::string id_;
    const double datarate_ = 1024.0; //default 1024 bits/sec
    const double buffer_size_ = 10240.0; //default 10kb 
    const double delay_ = 0.001; //default 1ms delay
    const Node& end1_;
    const Node& end2_;

    //variables
    double used_buffer_ = 0;
    double occupancy_ = 0; // used buffer / total buffer
    bool transmitting_ = false;
    Packet& packet_to_transmit_;
    int num_packs_in_buffer_ = 0; 
};
#endif
