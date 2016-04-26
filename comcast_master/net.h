/**
 * ECE408 
 * net.h
 * Purpose: Network Simulator Object
 * 
 * @author Kangqiao Lei
 * @version 0.2.0 04/19/16
 */

#ifndef NET_H
#define NET_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

// Forward declarations.
#include "events/event.h"
#include "util.h"
#include "host.h"
#include "router"

class flow;
class Link;
class node;

float simtime;

class net{
	private:
	int nflows;	//number of flows
	bool fiveever; // 5ever? or is there a stop time
	float endtime;	// end of simulation
	
<<<<<<< HEAD
	//Lists of objects for each class
	std::priority_queue<event, vector<event>, compareEvents> events;
	std::vector<host *> hosts; 
	std::vector<router *> routers; 
	std::vector<link *> links; 
	std::vector<flow *> flows; 
	
	public:
	//initializes net
	net();
	//clears net
	~net();
=======
	std::priority_queue<event*, std::vector<event*>, compareEvents> events;
	std::vector<host*> hosts; 
	std::vector<router *> routers; 
	std::vector<flow*> flows; 
	std::vector<Link*> links; 

	public: 
        net();
        ~net();
>>>>>>> 8b583dc3049d36ee176e40937169c7f1d9ee4fe5
	
        float getTime(){return simtime;};
	float setTime(float ntime){return simtime = ntime;};
	float getEnd(float stop){return endtime;};
	float setEnd(float stop){fiveever = true; return endtime = stop;};
	bool isEnd(float stop){return (fiveever && simtime > endtime);};
	
	// checks for existence of identically named objeccts of the same class
	bool nodeExists(std::string id);
	bool hostExists(std::string id);
	bool routerExists(std::string id);
	bool linkExists(std::string id);
	bool flowExists(std::string id);
	
	// gets the id of the object
	node* getNode(std::string id);
	host* getHost(std::string id);
	router* getRouter(std::string id);
	Link* getLink(std::string id);
	flow* getFlow(std::string id);
	
	// adds an object after verifying uniqueness
	int addHost(std::string id);
	int addRouter(std::string id);
	int addLink(std::string id, std::string node_id1, std::string node_id2, float rate, float delay, float buffer);
	int addFlow(std::string id, std::string node_src, std::string node_dst, float data_size, float start_time, TCP_type tcp_enum);
	
	// determines number of active flows
	int flowFinished();
<<<<<<< HEAD
	// insert into priority queue
	int addEvent(event e);
	// start the simulation
=======
	int addEvent(event*);
>>>>>>> 8b583dc3049d36ee176e40937169c7f1d9ee4fe5
	int run();
	
	// debug
	string print();
};
<<<<<<< HEAD
=======


>>>>>>> 8b583dc3049d36ee176e40937169c7f1d9ee4fe5
#endif
