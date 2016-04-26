/**
 * CommNets
 * @Author Eui Han
 */
#ifndef PACKET_H_
#define PACKET_H_
#include <string>
#include "node.h"

class Packet{
  private:
    const std::string id_;
    const std::string type_; //indicates if it is ACK or Data
    const Node* origin_;//sender of the packet(same as the sender of the flow)
    const Node* dest_;//destination of the packet (same as the destination of the flow)
    const int size_; //size of the packet. Unit: bits
    const int seq_num_; //The sequence number of the packet.
  public:
    //constructors
    Packet(const std::string id, const Node* src, const Node* dst);
    Packet(const std::string type, int seq, const Node* src, const Node* dst);
    
    //functions
    const Node* GetSrc() const; //returns the sender info
    const Node* GetDst() const; //returns the destination info
    const int size() const; //returns the size of the packet
    const int seqNum() const; //returns sequence number of this packet
    const std::string id() const;
};
#endif
