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
class host;
class flow;
class Link;
class node;

float simtime;

class net{
	private:
	int nflows;
	bool fiveever; // 5ever? or is there a stop time
	float endtime;
	
	std::priority_queue<event*, std::vector<event*>, compareEvents> events;
	std::vector<host*> hosts; 
	//std::vector<router *> routers; 
	std::vector<flow*> flows; 
	std::vector<Link*> links; 

	public: 
	net();
	~net();
	
        float getTime(){return simtime;};
	float setTime(float ntime){return simtime = ntime;};
	float getEnd(float stop){return endtime;};
	float setEnd(float stop){fiveever = true; return endtime = stop;};
	bool isEnd(float stop){return (fiveever && simtime > endtime);};
	
	bool nodeExists(std::string id);
	bool hostExists(std::string id);
	bool routerExists(std::string id);
	bool linkExists(std::string id);
	bool flowExists(std::string id);
	
	node* getNode(std::string id);
	host* getHost(std::string id);
	//router* getRouter(std::string id);
	Link* getLink(std::string id);
	flow* getFlow(std::string id);
	
	int addHost(std::string id);
	int addRouter(std::string id);
	int addLink(std::string id, std::string node_id1, std::string node_id2, float rate, float delay, float buffer);
	int addFlow(std::string id, std::string node_src, std::string node_dst, float data_size, float start_time, TCP_type tcp_enum);
	
	int flowFinished();
	int addEvent(event*);
	int run();
	
	void print();
};


#endif
