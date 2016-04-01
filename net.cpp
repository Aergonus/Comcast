/**
 * ECE408 
 * net.cpp
 * Purpose: Network Simulator Object
 * 
 * @author Kangqiao Lei
 * @version 0.1 03/28/16
 */

#include "net.h"

net::net(){
	time = 0;
	fiveever = false;
}

net::~net(){
	// Iterate through list and delete
	for(auto iter = hosts.begin(); iter != hosts.end(); ++iter){
		delete *iter;
	}
	hosts.clear();
	for(auto iter = routers.begin(); iter != routers.end(); ++iter){
		delete *iter;
	}
	routers.clear();
	for(auto iter = links.begin(); iter != links.end(); ++iter){
		delete *iter;
	}
	links.clear();
	//When flows reference TCP protocols, have to deallocate those too
	for(auto iter = flows.begin(); iter != flows.end(); ++iter){
		delete *iter;
	}
	flows.clear();
}

int net::addHost(std::string id){
	if (!nodeExists(id)){
		// Create a new host object
		hosts[id] = host(id);

		// Update relations
		nodes.push_back(hosts[id]);
		
		return 0;
	} else {
		errorSS << "Failed to create Host with id " << id << ". ID already exists." << endl;
		return -1;
	}
	
}

int net::addRouter(std::string id){
	if (!nodeExists(id)){
		// Create a new router object
		routers[id] = router(id);

		// Update relations
		nodes.push_back(routers[id]);
		
		// Initialization Routing Table Event
		event_updateRT *initialRT = new event_updateRT(0, *this); // Offset from start by -EPSILON?
		// This is a reoccuring even that adds a new event_updateRT every CONSTANT (util) RT_refresh
		// Note that the actual updates propogate as send/recieve packet events
		// Initial static cost??? @Arvind
		
		return 0;
	} else {
		errorSS << "Failed to create Router with id " << id << ". ID already exists." << endl;
		return -1;
	}
	
}

int net::addLink(std::string id, std::string node_id1, std::string node_id2, float rate, float delay, float buffer){
	// Check if there exists a link with this id already
	if (!linkExists(id)){
		// Check if both IDs are valid nodes
		if(nodeExists(node_id1) && nodeExists(node_id2)){
			// Get the nodes
			n1 = nodes[node_id1];
			n2 = nodes[node_id2];
		
			links[id] = Link(id, n1, n2, rate, delay, buffer);
		
			// Update relations
			n1.addLink(links[id]);
			n2.addLink(links[id]);

		} else if(!nodeExists(node_id1)){
			errorSS << "Failed to create Link with id " << id << ". Node " << node_id1 << " does not exist." << endl;
			return -1;
		} else if(nodeExists(node_id2)){
			errorSS << "Failed to create Link with id " << id << ". Node " << node_id2 << " does not exist." << endl;
			return -1;
		}
		return 0;
	} else {
		errorSS << "Failed to create Link with id " << id << ". ID already exists." << endl;
		return -1;
	}
}

// TODO: Add TCP param and alter addFlow accordingly
int net::addFlow(std::string id, std::node_src, std::node_dst, float data_size, float start_time){
	// Check if there exists a flow with this id already
	if (!flowExists(id)){
		// Check if both IDs are valid nodes
		if(nodeExists(node_src) && nodeExists(node_dst)){
			// Get the nodes
			src = nodes[node_src];
			dst = nodes[node_dst];
		
			//TODO: Implement TCP Algo
			flows[id] = flow(id, src, dst, data_size, start_time);
			nflows++;
		
			// Update relations
			src.addFlow(flows[id]);
			dst.addFlow(flows[id]);
			
			// TODO: Create initial events
			event_startFlow *flowStart = new flowStart(start_time, *this, flows[id]);
			// for TCP protocols update windows and other various events. Case statement here

		} else if(!nodeExists(node_src)){
			errorSS << "Failed to create Flow with id " << id << ". Node " << node_src << " does not exist." << endl;
			return -1;
		} else if(nodeExists(node_dst)){
			errorSS << "Failed to create Flow with id " << id << ". Node " << node_dst << " does not exist." << endl;
			return -1;
		}		
		return 0;
	} else {
		errorSS << "Failed to create Flow with id " << id << ". ID already exists." << endl;
		return -1;
	}
}

int net::flowFinished(){
	return --nflows;
}

int net::addEvent(event *e){
	e->setStart(e->getStart + time); // Adds global time to the initialized delay
	events.push(e);
}

int net::run(){
	// Ends if there are no flows left
	while ((!events.empty() && nFlows > 0)){
		//Simulation ends at user specified time
		if (isEnd()){
			break;
		}
		//TODO: Establish precedence order? Drain buffers before pushing to them etc
		Event *to_handle = events.top();
		time = to_handle->start;
		events.pop();
		to_handle->handle_event();
		delete to_handle;
	}
}