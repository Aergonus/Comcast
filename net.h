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
class event;
class Host;
class Router;
class Link;
class Flow;

// Custom Libraries
#include "events/event.h"
#include "util.h"

float simtime;

class net {
	private:
	int nFlows;	//number of Flows
	bool fiveever; // 5ever? or is there a stop time
	float endtime;	// end of simulation
	
	//Lists of objects for each class
	std::priority_queue<event, vector<event>, compareEvents> events;
	std::vector<Host *> Hosts; 
	std::vector<Router *> Routers; 
	std::vector<Link *> Links; 
	std::vector<Flow *> Flows; 
	
	public:
	//initializes net
	net();
	//clears net
	~net();
	
	//float getTime(){return time;};
	float setTime(float ntime){return time = ntime;};
	float getEnd(float stop){return endtime;};
	float setEnd(float stop){fiveever = true; return endtime = stop;};
	bool isEnd(float stop){return (fiveever && time > endtime);};
	
	// checks for existence of identically named objeccts of the same class
	bool NodeExists(std::string id);
	bool HostExists(std::string id);
	bool RouterExists(std::string id);
	bool LinkExists(std::string id);
	bool FlowExists(std::string id);
	
	// gets the id of the object
	Node* getNode(std::string id);
	Host* getHost(std::string id);
	Router* getRouter(std::string id);
	Link* getLink(std::string id);
	Flow* getFlow(std::string id);
	
	// adds an object after verifying uniqueness
	int addHost(std::string id);
	int addRouter(std::string id);
	int addLink(std::string id, std::string Node_id1, std::string Node_id2, float rate, float delay, float buffer);
	int addFlow(std::string id, std::string Node_src, std::string Node_dst, float data_size, float start_time, TCP_type tcp_enum);
	
	// determines number of active Flows
	int FlowFinished();
	// insert into priority queue
	int addEvent(event e);
	// start the simulation
	int run();
	
	// debug
	string print();
};
#endif
