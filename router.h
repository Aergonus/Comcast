/**
 * ECE408 
 * Router.h
 * Purpose: 
 * 
 * @author Eui Han, Arvind Nagalingam
 * @version 0.2.0 04/19/16
 */
#ifndef ROUTER_H
#define ROUTER_H

class net;
class Packet;
class Link;
#include "Node.h"
#include <map>

class Router: public Node{
  public:
	Router(std::string id);
	void receive_pak(Packet *p);
	Link* get_route(std::string); // looks up the routing table and returns the Link
	void update_table(std::string); // updates the routing table every x time step
	void update_cost();
	void send_control() ;
	void receive_control(Packet* p);
  private:
	//each row represents each Router's dist_ + cost_ vector
	std::map<std::string, std::map<std::string, double> > routing_table;

	//map <neighbor id, Link cost>
	std::map<std::string, double> costVec;
	
	//map <hst id, neighbor id>
	std::map<std::string, std::string> next_hop;

	//map <Node id, pointer to Link>
	std::map<std::string, Link*> Links;

	//map <Router id, pointer to the Router>
	std::map<std::string, Router*> neighbors;
};
#endif
