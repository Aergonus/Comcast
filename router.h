/**
 * ECE408 
 * Router.h
 * Purpose: 
 * 
 * @author Eui Han, Arvind Nagalingam, Kangqiao Lei
 * @version 0.2.0 04/19/16
 */
#ifndef ROUTER_H
#define ROUTER_H

class net;
class Packet;
class Link;

#include "node.h"
#include <map>

class Router: public Node{
  private:
	// Network Simulator 
	net *Network;

	// Maps the Host id to the Link id
	std::map<std::string, std::string> next_hop;

	// Maps id of a Link to the connected Node
	std::map<std::string, Node*> neighbors;

  public:
    Router(std::string id): Node(id){};
	Router(std::string id, net *sim): Node(id), Network(sim){
		isRouter = true;
	};
	~Router(){};
	// Add a Link for the Router
	void addLink(Link *l);
	Node* getConnectedNode(Link *);
	
	// Receives incoming Packet
	void receive_pak(Packet *p);
	
	// Debug
	void print();

	bool operator == (Router *cmpRouter){
		return (this->getName() == cmpRouter->getName());
	};

};
#endif
