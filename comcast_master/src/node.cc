/**
 * CommNets
 * node.cc
 * 
 * @author Eui Han
 * @version 0.1 04/05/16
 */

#include "node.h"

Node::Node(consts td::string id) : id_(id){}

void Node::AddNeighbor(const Node &n, const Link &l){
  neighbors_.insert({n.id(),l});
  AddLink(Link()) // TODO:add default link spec
}

void Node::AddLink(const Link &l){
  links_.push_back(l);
}

void Node::AddNode(const Node &n){
  nodes_.push_back(n);
}
const std::string Node::GetId(){
  return id_;
}
std::vector<links> Node::GetLinks(){
  return links_;
}

std::vector<Node> Node::GetConnectedNodes(){
  return nodes_;
}


virtual void Node::SendPacket(Packet &p, double time, Link &l, Node &dst){}
virtual void Node::ReceivePacket(Packet &p, double time){}


  
