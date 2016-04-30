/**
 * ECE408 
 * net.cpp
 * Purpose: Network Simulator Object
 * 
 * @author Kangqiao Lei
 * @version 0.2.0 04/19/16
 */

#include "net.h"
#include "host.h"
#include "router.h"
#include "link.h"
#include "flow.h"
#include "events/event_start_flow.h"

float simtime;

// Initialize
net::net(){
	simtime = 0;
	fiveever = false;
}

// Clears the net
net::~net(){
	// Iterate through list and delete
	for(auto iter = Hosts.begin(); iter != Hosts.end(); ++iter){
		delete *iter;
	}
	Hosts.clear();
	for(auto iter = Routers.begin(); iter != Routers.end(); ++iter){
		delete *iter;
	}
	Routers.clear();
	for(auto iter = Links.begin(); iter != Links.end(); ++iter){
		delete *iter;
	}
	Links.clear();
	//When Flows reference TCP protocols, have to deallocate those too
	for(auto iter = Flows.begin(); iter != Flows.end(); ++iter){
		delete *iter;
	}
	Flows.clear();
}

//Get statements for each class
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
	while(itr != Hosts.end()) {
		if ((*itr)->getName() == id) {
			return *itr;
		}
		itr++;
	}
	return NULL;
}

Router* net::getRouter(std::string id){
	std::vector<Router *>::iterator itr = Routers.begin();
	while(itr != Routers.end()) {
		if ((*itr)->getName() == id) {
			return *itr;
		}
		itr++;
	}
	return NULL;
}

Link* net::getLink(std::string id){
	std::vector<Link *>::iterator itr = Links.begin();
	while(itr != Links.end()) {
		if ((*itr)->getName() == id) {
			return *itr;
		}
		itr++;
	}
	return NULL;
}

Flow* net::getFlow(std::string id){
	std::vector<Flow *>::iterator itr = Flows.begin();
	while(itr != Flows.end()) {
		if ((*itr)->getName() == id) {
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


// Add functions for all the classes
int net::addHost(std::string id){
	if (!NodeExists(id)){
		// Create a new Host object
		Host *newHost = new Host(id);

		// Update relations
		Hosts.push_back(newHost);
		//Nodes.push_back(newHost);
		
		return 0;
	} else {
		*errorSS << "Failed to create Host with id " << id << ". ID already exists." << std::endl;
		return -1;
	}
	
}

int net::addRouter(std::string id){
	if (!NodeExists(id)){
		// Create a new Router object
		Router *newRouter = new Router(id, this);

		// Update relations
		Routers.push_back(newRouter);
		//Nodes.push_back(newRouter);
		
		// Initialization Routing Table Event
		// event_updateRT *initialRT = new event_updateRT(0, *this); // Offset from start by -EPSILON?
		// This is a reoccuring even that adds a new event_updateRT every CONSTANT (util) RT_refresh
		// Note that the actual updates propogate as send/recieve Packet events
		// Initial static cost??? @Arvind
		
		return 0;
	} else {
		*errorSS << "Failed to create Router with id " << id << ". ID already exists." << std::endl;
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
			n1->addLink(getLink(id));
			n2->addLink(getLink(id));

		} else if(!NodeExists(Node_id1)){
			*errorSS << "Failed to create Link with id " << id << ". Node " << Node_id1 << " does not exist." << std::endl;
			return -1;
		} else if(NodeExists(Node_id2)){
			*errorSS << "Failed to create Link with id " << id << ". Node " << Node_id2 << " does not exist." << std::endl;
			return -1;
		}
		return 0;
	} else {
		*errorSS << "Failed to create Link with id " << id << ". ID already exists." << std::endl;
		return -1;
	}
}

// TODO: Add TCP param and alter addFlow accordingly
int net::addFlow(std::string id, std::string Node_src, std::string Node_dst, float data_size, float start_time, TCP_type tcp_enum){
	// Check if there exists a Flow with this id already
	if (!FlowExists(id)){
		// Check if both IDs are valid Nodes
		if(NodeExists(Node_src) && NodeExists(Node_dst)){
			// Get the Nodes
			Host *src = getHost(Node_src);
			Host *dst = getHost(Node_dst);
		
			//TODO: Implement TCP Algo
			Flow *newFlow = new Flow(id, src, dst, data_size, start_time, tcp_enum, this);
			nFlows++;
		
			// Update relations
			Flows.push_back(newFlow);
			src->addFlow(getFlow(id));
			dst->addFlow(getFlow(id));
			
			// TODO: Create initial events
			event_start_flow *FlowStart = new event_start_flow(start_time, getFlow(id));
			addEvent(FlowStart);
			// for TCP protocols update windows and other various events. Case statement here

		} else if(!NodeExists(Node_src)){
			*errorSS << "Failed to create Flow with id " << id << ". Node " << Node_src << " does not exist." << std::endl;
			return -1;
		} else if(NodeExists(Node_dst)){
			*errorSS << "Failed to create Flow with id " << id << ". Node " << Node_dst << " does not exist." << std::endl;
			return -1;
		}		
		return 0;
	} else {
		*errorSS << "Failed to create Flow with id " << id << ". ID already exists." << std::endl;
		return -1;
	}
}

// Decrement the number of active Flows when one is done transmitting
int net::FlowFinished(){
	return --nFlows;
}

// Priority queue
void net::addEvent(event *e){
	e->set_Start(e->get_Start() + simtime); // Adds global time to the initialized delay
	events.push(e);
}

// Runs the simulation
int net::run(){
	// Ends if there are no Flows left
	while ((!events.empty() && nFlows > 0)){
	*errorSS << "Running!" << std::endl;
		//Simulation ends at user specified time
		if (isEnd()){
			*errorSS << "I'm broken!" << std::endl;
			break;
		}
		//TODO: Establish precedence order? Drain buffers before pushing to them etc
		event *to_handle = events.top();
	*errorSS << "Eh!" << std::endl;
		if(to_handle->isValid()) {
			simtime = to_handle->get_Start();
			*errorSS << "Handling!" << std::endl;
			to_handle->handle_event();
		}
	*errorSS << "Ready to pop!" << std::endl;	
		events.pop();
		delete to_handle;
	*errorSS << "Dead!" << std::endl;
	}
	return 0;
}
