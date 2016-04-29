/**
 * ECE408 
 * net.cpp
 * Purpose: Network Simulator Object
 * 
 * @author Kangqiao Lei
 * @version 0.2.0 04/19/16
 */

#include "net.h"
#include "events/event_start_Flow.h"

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

//Overloading  == operators
//Return true if two objects have same id
bool operator == (const &Node a, const &Node b){
    return (a.getID() == b.getID());
}

bool operator == (const &Link a, const &Link b){
    return (a.getID() == b.getID());
}

bool operator == (const &Flow a, const &Flow b){
    return (a.getID() == b.getID());
}

//Get statements for each class
Node* getNode(std::string id){
    if (HostExists(id)){
        return getHost(id);
    }
    else if (RouterExists(id)){
        return getRouter(id);
    }
    else
        return NULL;
}

Host* getHost(std::string id){
    std::vector<Host>::iterator itr = std::find(Hosts.begin(), Hosts.end(), Host(id)) 
    if (itr == Hosts.end()){
        return NULL;
    }
    return &(*itr);
}

Router* getRouter(std::string id){
    std::vector<Router>::iterator itr = std::find(Routers.begin(), Routers.end(), Router(id));
    if (itr == Routers.end()){
        return NULL;
    }
    return &(*itr);
}

Link* getLink(std::string id){
    std::vector<Link>::iterator itr = std::find(Links.begin(), Links.end(), Link(id));
    if (itr == Link.end()){
        return NULL;
    }
    return &(*itr);
}

Flow* getFlow(std::string id){
    std::vector<Flow>::iterator itr = std::find(Flows.begin(), Flows.end(), Flow(id));
    if (itr == Flows.end()){
        return NULL;
    }
    return &(*itr);
}

// Checks existence of identically labeled objects
bool NodeExists(std::string id){
    return (getNode(id) != NULL);    
}

bool HostExists(std::string id){
    return (getHost(id) != NULL);    
}

bool RouterExists(std::string id){
    return (getRouter(id) != NULL);
}

bool LinkExists(std::string id){
    return (getLink(id) !=NULL);
}

bool FlowExists(std::string id){
    return (getFlow(id) != NULL);
}


// Add functions for all the classes
int net::addHost(std::string id){
	if (!NodeExists(id)){
		// Create a new Host object
		Hosts[id] = Host(id);

		// Update relations
		Nodes.push_back(Hosts[id]);
		
		return 0;
	} else {
		errorSS << "Failed to create Host with id " << id << ". ID already exists." << endl;
		return -1;
	}
	
}

int net::addRouter(std::string id){
	if (!NodeExists(id)){
		// Create a new Router object
		Routers[id] = Router(id);

		// Update relations
		Nodes.push_back(Routers[id]);
		
		// Initialization Routing Table Event
		event_updateRT *initialRT = new event_updateRT(0, *this); // Offset from start by -EPSILON?
		// This is a reoccuring even that adds a new event_updateRT every CONSTANT (util) RT_refresh
		// Note that the actual updates propogate as send/recieve Packet events
		// Initial static cost??? @Arvind
		
		return 0;
	} else {
		errorSS << "Failed to create Router with id " << id << ". ID already exists." << endl;
		return -1;
	}
	
}

int net::addLink(std::string id, std::string Node_id1, std::string Node_id2, float rate, float delay, float buffer){
	// Check if there exists a Link with this id already
	if (!LinkExists(id)){
		// Check if both IDs are valid Nodes
		if(NodeExists(Node_id1) && NodeExists(Node_id2)){
			// Get the Nodes
			Node *n1 = Nodes[Node_id1];
			Node *n2 = Nodes[Node_id2];
		
			Links[id] = Link(id, n1, n2, rate, delay, buffer);
		
			// Update relations
			n1->addLink(Links[id]);
			n2->addLink(Links[id]);

		} else if(!NodeExists(Node_id1)){
			errorSS << "Failed to create Link with id " << id << ". Node " << Node_id1 << " does not exist." << endl;
			return -1;
		} else if(NodeExists(Node_id2)){
			errorSS << "Failed to create Link with id " << id << ". Node " << Node_id2 << " does not exist." << endl;
			return -1;
		}
		return 0;
	} else {
		errorSS << "Failed to create Link with id " << id << ". ID already exists." << endl;
		return -1;
	}
}

// TODO: Add TCP param and alter addFlow accordingly
int net::addFlow(std::string id, std::Node_src, std::Node_dst, float data_size, float start_time, TCP_type tcp_enum){
	// Check if there exists a Flow with this id already
	if (!FlowExists(id)){
		// Check if both IDs are valid Nodes
		if(NodeExists(Node_src) && NodeExists(Node_dst)){
			// Get the Nodes
			src = Nodes[Node_src];
			dst = Nodes[Node_dst];
		
			//TODO: Implement TCP Algo
			Flows[id] = Flow(id, src, dst, data_size, start_time, tcp_enum);
			nFlows++;
		
			// Update relations
			src.addFlow(Flows[id]);
			dst.addFlow(Flows[id]);
			
			// TODO: Create initial events
			event_start_Flow *FlowStart = new FlowStart(start_time, *this, Flows[id]);
			// for TCP protocols update windows and other various events. Case statement here

		} else if(!NodeExists(Node_src)){
			errorSS << "Failed to create Flow with id " << id << ". Node " << Node_src << " does not exist." << endl;
			return -1;
		} else if(NodeExists(Node_dst)){
			errorSS << "Failed to create Flow with id " << id << ". Node " << Node_dst << " does not exist." << endl;
			return -1;
		}		
		return 0;
	} else {
		errorSS << "Failed to create Flow with id " << id << ". ID already exists." << endl;
		return -1;
	}
}

// Decrement the number of active Flows when one is done transmitting
int net::FlowFinished(){
	return --nFlows;
}

// Priority queue
int net::addEvent(event *e){
	e->setStart(e->getStart + time); // Adds global time to the initialized delay
	events.push(e);
}

// Runs the simulation
int net::run(){
	// Ends if there are no Flows left
	while ((!events.empty() && nFlows > 0)){
		//Simulation ends at user specified time
		if (isEnd()){
			break;
		}
		//TODO: Establish precedence order? Drain buffers before pushing to them etc
		Event *to_handle = events.pop();//events.top();
		if(to_handle.valid) {
			time = to_handle->start;
			to_handle->handle_event();
		}
		delete to_handle;
	}
	return 0;
}
