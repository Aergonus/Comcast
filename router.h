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
	
    //map <neighbor id, link cost>
    std::map<std::string, float> costVec;
	
	// ------------------------------------------------------------------------
    float ctrl_Updated; //keep track of the time last control packet was sent

    Link* received_from_;
	
    //each row represents each router's dist_ + cost_ vector
    std::map<std::string, std::map<std::string, float> > routing_table_;

	// ------------------------------------------------------------------------
	
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
	
	// ------------------------------------------------------------------------
    Router(std::string id);
    virtual void SendPacket(Packet, float);
    virtual void ReceivePacket(Link&, Packet, float);
    bool allowedToTransmit();
    Link& GetRoute(std::string); // looks up the routing table and returns the link (first link ref is the link back to the src)
    void UpdateTable(std::string); // updates the routing table every x time step
    void UpdateCost();
    void SendControl();
    void ReceiveControl(Packet p);
    std::map<std::string, float> RoutingVector() const;
    Link& Greedy(std::string);
    void Init(); //initializes the routing table
	// ------------------------------------------------------------------------
	
	// Debug
	void print();

	bool operator == (Router *cmpRouter){
		return (this->getName() == cmpRouter->getName());
	};

};
#endif
