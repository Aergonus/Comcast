/**
 * CommNets
 * @Author Eui Han
 */
#include <string>
#include <sstream>
#include "packet.h"

Packet::Packet(const std::string id, const Node* src, const Node* dst){
  id_ = id;
  type_ = id[0];
  origin_ = src;
  dest_ = dst;
  seq_num_ = atoi(id.substr(1));
}

Packet::Packet(const std::string type, int seq, const Node* src, const Node* dst){
  sstream out << seq;
  type_ = type + out.str();
  seq_ = seq;
  src_ = src;
  dst_ = dst;
}

const Node* Packet::GetSrc() const{
  return src;
}

const std::string id() const{
  return id_;
}

const Node* Packet::GetDest() const{
  return dest_;
}
const int Packet::size() const{
  return size_;
}
const int Packet::seqNum() const{
  return seq_num_;
}
