/**
 * ECE408 
 * Router.h
 * Purpose: 
 * 
 * @author Eui Han, Arvind Nagalingam, Kangqiao Lei
 * @version 0.5.0 05/03/16
 */
#ifndef ROUTER_H
#define ROUTER_H

class net;
class Packet;
class Link;

#include "node.h" // includes map

class Router: public Node{
  private:
	// Network Simulator 
	net *Network;

	float ctrl_Updated; //keep track of the time last control packet was sent

/*
			DVT
	H1	H2	H3	...	Hn	(ALL HOSTS in network)
me 	datadatadata...	data 
R1	datadatadata...	data
R2	datadatadata...	data
R3	datadatadata...	data
...
Rm
(neighbors routers ONLY)
	Data contains distance to host
//each row represents each router's dist_ + cost_ vector
*/
	std::map<std::string, std::map<std::string, float> > routing_table;
	std::map<std::string, float> updatedDVTime; // Time of update to each DV in routing table
	
/* 
			NH Lookup
	H1	H2	H3	...	Hn	(ALL HOSTS in network)
me 	n	n	n	...	node
//Maps Host id to Link id
*/
	std::map<std::string, std::string> next_hop;
	
/*
			CV
me	R1	R2	R3	...	Rm	H?		(All Neighbors)
0 	datadatadata...	datadata	Data contains cost to neighbor 
*/
	std::map<std::string, float> costVec;
	
	void receiveControl(Packet *p, Link *l);
	bool updateCost();
	void fwdPak(Packet *p, Link *fromLink);
	void sendCtrl();
	
  public:
	Router(std::string id): Node(id){};
	Router(std::string id, net *sim): Node(id), Network(sim){
		isRouter = true;
	};
	~Router(){};
	
	// Receives incoming Packet
	void receive_pak(Packet *p, Link *l);
	
	// Initializes Routing Table
	void init();
	
	// Debug
	void print(){};

	bool operator == (Router *cmpRouter){
		return (this->getName() == cmpRouter->getName());
	};

};
#endif