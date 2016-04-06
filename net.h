/**
 * ECE408 
 * net.h
 * Purpose: Network Simulator Object
 * 
 * @author Kangqiao Lei
 * @version 0.1.5 04/05/16
 */

#ifndef NET_H
#define NET_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

// Forward declarations.
class event;
class host;
class router;
class link;
class flow;

// External Variables 
extern bool debug;
extern ostream &debugSS;
extern ostream &errorSS;

// Custom Libraries
#include "event.h"

class net {
	private:
	int nflows;
	bool fiveever; // 5ever? or is there a stop time
	float time, endtime;
	
	std::priority_queue<event, vector<event>, compareEvents> events;
	std::vector<host *> hosts; 
	std::vector<router *> routers; 
	std::vector<link *> links; 
	std::vector<flow *> flows; 
	
	public: 
	net();
	~net();
	
	float getTime(){return time;};
	float setTime(float ntime){return time = ntime;};
	float getEnd(float stop){return endtime;};
	float setEnd(float stop){fiveever = true; return endtime = stop;};
	bool isEnd(float stop){return (fiveever && time > endtime);};
	
	bool nodeExists(std::string id);
	bool hostExists(std::string id);
	bool routerExists(std::string id);
	bool linkExists(std::string id);
	bool flowExists(std::string id);
	
	node* getNode(std::string id);
	host* getHost(std::string id);
	router* getRouter(std::string id);
	link* getLink(std::string id);
	flow* getFlow(std::string id);
	
	int addHost(std::string id);
	int addRouter(std::string id);
	int addLink(std::string id, std::string node_id1, std::string node_id2, float rate, float delay, float buffer);
	int addFlow(std::string id, std::string node_src, std::string node_dst, float data_size, float start_time);
	
	int flowFinished();
	int addEvent(event e);
	int run();
	
	void print();
};
#endif