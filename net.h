/**
 * ECE408 
 * net.h
 * Purpose: Network Simulator Object
 * 
 * @author Kangqiao Lei
 * @version 0.1 03/28/16
 */

#ifndef NET_H
#define NET_H
#include <algorithm>

#include <vector>
#include <queue>
#include "link.h"
#include "flow.h"
#include "host.h"
#include "event.h" 

class net {
	private:
	float time, endtime, nflows;
	//std::vector<std::string> ids; 
	//std::vector<node *> nodes; 
	
	std::priority_queue<event, vector<event>, compareEvents> events;
	std::vector<host> hosts; 
	std::vector<router> routers; 
	std::vector<link> links; 
	std::vector<flow> flows; 
	
	public: 
	net();
	
	float getTime(){return time;}
	float setEnd(float stop);
	
	bool usedID(std::string id);
	
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
	int addEvent(event e, float delay);
	int run();
	
	void print();
}