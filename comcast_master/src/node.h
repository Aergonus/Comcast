/**
 * CommNets
 * node.h
 * Purpose: Nodes are points of the network, i.e. hosts and routers
 * 
 * @author Eui Han
 * @version 0.1 04/05/16
 */

#ifndef NODE_H_
#define NODE_H_

#include <unordered_map>
#include <vector>
class Link;
class Packet;

class Node{
  public: 
    Node(const std::string id);
    void AddLink(Link&);
    void AddNode(Node&);
    const std::string id();
    std::vector<Link> GetLinks();
    std::vector<Node> GetConnectedNodes();
    virtual void SendPacket(Packet&, double);
    virtual void RecievePacket(Packet&, double);

  protected:
    int bits_sent_=0;
    const std::string id_;
    std::unordered_map<std::string, Link&> neighbors_; 
    std::vector<Node> nodes_;
    std::vector<Link> links_;
    std::vector<Packet> received_packets_;
    // bool routerorhostnode?
};

#endif
