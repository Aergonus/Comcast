#ifndef ROUTER_H_
#define ROUTER_H_

#include "node.h"
#include <vector>
#include <map>
class Network;
class Packet;

class Router: public Node{
  public:
    Router(const std::string id);
    void SendPacket(const Packet &p, const double time);
    void RecievePacket(const Packet &p, const double time);
    bool allowedToTransmit();
    link& GetRoute(Host&); // looks up the routing table and returns the link
    void UpdateTable(Host&); // updates the routing table every x time step

  private:
    // <link id, link cost>
    std::unordered_map<const std::string, double> cost_; 

    //<host id, total link cost> distance from me to another host
    std::unordered_map<const std::string, double> distance_; 

    //<host id, link id> route to the destination
    std::unordered_map<const std::string, const std::string> next_hop_; 

    //<host id, link id> distance vector of other hosts
    std::unordered_map<const std::string, std::unordered_map<std::string, double> > routing_table_; 
};
#endif
