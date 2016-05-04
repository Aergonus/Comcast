/**
 * ECE408 
 * net.cpp
 * Purpose: Network Simulator Object
 * 
 * @author Kangqiao Lei
 * @version 0.5.0 05/03/16
 */

#include <cfloat>
#include "net.h"
#include "link.h"
#include "flow.h"
#include "node.h"
#include "host.h"
#include "router.h"
#include "events/event_log.h"
#include "events/event_init_rt.h"
#include "events/event_start_flow.h"

// Global Variables
float simtime = FLT_MIN;
int eventsHandled;
int eventsCreated;

// Initialize
net::net(){
	fiveever = false;
}

// Clears the net
net::~net(){
	// Iterate through list and delete
	for(auto iter = Hosts.begin();iter != Hosts.end();++iter){
		delete *iter;
	}
	Hosts.clear();
	for(auto iter = Routers.begin();iter != Routers.end();++iter){
		delete *iter;
	}
	Routers.clear();
	for(auto iter = Links.begin();iter != Links.end();++iter){
		delete *iter;
	}
	Links.clear();
	for(auto iter = Flows.begin();iter != Flows.end();++iter){
		delete *iter;
	}
	Flows.clear();
}

// Get statements for each class
Node* net::getNode(std::string id){
	if (HostExists(id)){
		return getHost(id);
	}
	else if (RouterExists(id)){
		return getRouter(id);
	}
	else
		return NULL;
}

Host* net::getHost(std::string id){
	std::vector<Host *>::iterator itr = Hosts.begin();
	while(itr != Hosts.end()){
		if ((*itr)->getName() == id){
			return *itr;
		}
		itr++;
	}
	return NULL;
}

Router* net::getRouter(std::string id){
	std::vector<Router *>::iterator itr = Routers.begin();
	while(itr != Routers.end()){
		if ((*itr)->getName() == id){
			return *itr;
		}
		itr++;
	}
	return NULL;
}

Link* net::getLink(std::string id){
	std::vector<Link *>::iterator itr = Links.begin();
	while(itr != Links.end()){
		if ((*itr)->getName() == id){
			return *itr;
		}
		itr++;
	}
	return NULL;
}

Flow* net::getFlow(std::string id){
	std::vector<Flow *>::iterator itr = Flows.begin();
	while(itr != Flows.end()){
		if ((*itr)->getName() == id){
			return *itr;
		}
		itr++;
	}
	return NULL;
}

// Checks existence of identically labeled objects
bool net::NodeExists(std::string id){
	return (getNode(id) != NULL);
}

bool net::HostExists(std::string id){
	return (getHost(id) != NULL);
}

bool net::RouterExists(std::string id){
	return (getRouter(id) != NULL);
}

bool net::LinkExists(std::string id){
	return (getLink(id) != NULL);
}

bool net::FlowExists(std::string id){
	return (getFlow(id) != NULL);
}

void net::init_Routing(){
	for (auto router:Routers){
		router->init();
	}
}

// Add functions for all the classes
int net::addHost(std::string id){
	if (!NodeExists(id)){
		// Create a new Host object
		Host *newHost = new Host(id);

		// Update relations
		Hosts.push_back(newHost);
		// Nodes.push_back(newHost);
		
		return 0;
	} else {
		*errorSS<<"Failed to create Host with id "<<id<<". ID already exists."<<std::endl;
		return -1;
	}
	
}

int net::addRouter(std::string id){
	if (!NodeExists(id)){
		// Create a new Router object
		Router *newRouter = new Router(id, this);

		// Update relations
		Routers.push_back(newRouter);
		// Nodes.push_back(newRouter);

		return 0;
	} else {
		*errorSS<<"Failed to create Router with id "<<id<<". ID already exists."<<std::endl;
		return -1;
	}
	
}

int net::addLink(std::string id, std::string Node_id1, std::string Node_id2, float rate, float delay, float buffer){
	// Check if there exists a Link with this id already
	if (!LinkExists(id)){
		// Check if both IDs are valid Nodes
		if(NodeExists(Node_id1) && NodeExists(Node_id2)){
			// Get the Nodes
			Node *n1 = getNode(Node_id1);
			Node *n2 = getNode(Node_id2);
		
			Link *newLink = new Link(id, n1, n2, rate, delay, buffer, this);
		
			// Update relations
			Links.push_back(newLink);
			n1->addNeighbor(newLink, n2);
			n2->addNeighbor(newLink, n1);

		} else if(!NodeExists(Node_id1)){
			*errorSS<<"Failed to create Link with id "<<id<<". Node "<<Node_id1<<" does not exist."<<std::endl;
			return -1;
		} else if(NodeExists(Node_id2)){
			*errorSS<<"Failed to create Link with id "<<id<<". Node "<<Node_id2<<" does not exist."<<std::endl;
			return -1;
		}
		return 0;
	} else {
		*errorSS<<"Failed to create Link with id "<<id<<". ID already exists."<<std::endl;
		return -1;
	}
}

int net::addFlow(std::string id, std::string Node_src, std::string Node_dst, float data_size, float start_time, TCP_type tcp_enum){
	// Check if there exists a Flow with this id already
	if (!FlowExists(id)){
		// Check if both IDs are valid Nodes
		if(NodeExists(Node_src) && NodeExists(Node_dst)){
			// Get the Nodes
			Host *src = getHost(Node_src);
			Host *dst = getHost(Node_dst);
		
			Flow *newFlow = new Flow(id, src, dst, data_size, start_time, tcp_enum, this);
			nFlows++;
		
			// Update relations
			Flows.push_back(newFlow);
			src->addFlow(newFlow);
			dst->addFlow(newFlow);
			
			event_start_flow *FlowStart = new event_start_flow(start_time, getFlow(id));
			addEvent(FlowStart);
		} else if(!NodeExists(Node_src)){
			*errorSS<<"Failed to create Flow with id "<<id<<". Node "<<Node_src<<" does not exist."<<std::endl;
			return -1;
		} else if(NodeExists(Node_dst)){
			*errorSS<<"Failed to create Flow with id "<<id<<". Node "<<Node_dst<<" does not exist."<<std::endl;
			return -1;
		}		
		return 0;
	} else {
		*errorSS<<"Failed to create Flow with id "<<id<<". ID already exists."<<std::endl;
		return -1;
	}
}

int net::FlowFinished(){
// Decrement the number of active Flows when one is done transmitting
	return --nFlows;
}

void net::addEvent(event *e){
	e->set_Start(e->get_Start() + simtime); // Adds global time to the initialized delay
	events.push(e);
	if (debug){
		*debugSS<<"CreateEvent,"<<simtime<<",";
		e->print();
	}
}

void net::vomitEvents(){
// Careful using this. It will cause the run while loop to segfault if called within there (which you probably will)
	if (debug){
		while(!events.empty()){
			event *to_handle = events.top();
			to_handle->print();
			events.pop();
			delete to_handle;
		}
	}
}

void net::log_Throughput(){
	for(auto&& f:Flows){
		f->logFlowRate();
	}
	for(auto&& l:Links){
		l->logLinkRate();
	}
	event_log *e = new event_log(LOGGING_INTERVAL,this);
	addEvent(e);
};

// Runs the simulation
int net::run(){
	// Create initial logging event
	event_log *e = new event_log(LOGGING_INTERVAL,this);
	addEvent(e);
	
	// Initialization Routing Table Event
	event_init_rt *initialRT = new event_init_rt(-EPSILON, this);
	addEvent(initialRT);

	// Ends if there are no Flows left
	while ((!events.empty() && nFlows > 0)){
		if (debug){
			*debugSS<<"Running!,"<<simtime<<",hEvents,"<<eventsHandled<<",nEvents,"<<events.size()<<std::endl;
		}
		
		// Simulation ends at user specified time
		if (isEnd()){
			if (debug){
				*debugSS<<"EndSim,"<<simtime<<",I'm broken!"<<std::endl;
			}
			break;
		}
		
		// TODO:Establish precedence order? Drain buffers before pushing to them etc
		event *to_handle = events.top();
		if(to_handle->isValid()){
			if (debug){
				*debugSS<<"Valid,"<<simtime<<",";
						to_handle->print();
				*debugSS<<"UpdateTime,"<<simtime<<",Changing Simulation Time to,"<<to_handle->get_Start()<<std::endl;
			}
			
			simtime = to_handle->get_Start();
			to_handle->handle_event();
		} else {
			if (debug){
				*debugSS<<"Invalid,"<<simtime<<",";
						to_handle->print();
			}
		}
		
		// If use vomitEvents debug, this will fail
		events.pop();
		delete to_handle;
		eventsHandled++;
	}

	if (debug){
		*debugSS<<"EndSimulation,"<<simtime<<std::endl;
	}
	return 0;
}
