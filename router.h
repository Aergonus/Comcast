/**
 * ECE408 
 * router.h
 * Purpose: 
 * 
 * @author Eui Han, Arvind Nagalingam
 * @version 0.2.0 04/19/16
 */
#ifndef ROUTER_H
#define ROUTER_H

class net;
class packet;
class link;
#include "node.h"
#include <map>

class router: public node{
  public:
    router(std::string id);
    void receive_pak(packet *p);
    link* get_route(std::string); // looks up the routing table and returns the link
    void update_table(std::string); // updates the routing table every x time step
    void update_cost();
    void send_control() ;
    void receive_control(packet* p);
  private:
    //each row represents each router's dist_ + cost_ vector
    std::map<std::string, std::map<std::string, double> > routing_table;

    //map <neighbor id, link cost>
    std::map<std::string, double> costVec;
    
    //map <hst id, neighbor id>
    std::map<std::string, std::string> next_hop;

    //map <node id, pointer to Link>
    std::map<std::string, link*> links;

    //map <router id, pointer to the router>
    std::map<std::string, router*> neighbors;
};
#endif
