/**
 * ECE408 
 * net.cpp
 * Purpose: Network Simulator Object
 * 
 * @author Kangqiao Lei
 * @version 0.2.0 04/19/16
 */

#include "net.h"
#include "event_start_flow.h"

// Initialize
net::net(){
	simtime = 0;
	fiveever = false;
}

// Clears the net
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

//Overloading  == operators
//Return true if two objects have same id
bool operator == (const &node a, const &node b){
    return (a.getID() == b.getID());
}

bool operator == (const &link a, const &link b){
    return (a.getID() == b.getID());
}

bool operator == (const &flow a, const &flow b){
    return (a.getID() == b.getID());
}

//Get statements for each class
node* getNode(std::string id){
    if (hostExists(id)){
        return getHost(id);
    }
    else if (routerExists(id)){
        return getRouter(id);
    }
    else
        return NULL;
}

host* getHost(std::string id){
    std::vector<host>::iterator itr = std::find(hosts.begin(), hosts.end(), host(id)) 
    if (itr == hosts.end()){
        return NULL;
    }
    return &(*itr);
}

router* getRouter(std::string id){
    std::vector<router>::iterator itr = std::find(routers.begin(), routers.end(), router(id));
    if (itr == routers.end()){
        return NULL;
    }
    return &(*itr);
}

link* getLink(std::string id){
    std::vector<link>::iterator itr = std::find(links.begin(), links.end(), link(id));
    if (itr == link.end()){
        return NULL;
    }
    return &(*itr);
}

flow* getFlow(std::string id){
    std::vector<flow>::iterator itr = std::find(flows.begin(), flows.end(), flow(id));
    if (itr == flows.end()){
        return NULL;
    }
    return &(*itr);
}

// Checks existence of identically labeled objects
bool nodeExists(std::string id){
    return (getNode(id) != NULL);    
}

bool hostExists(std::string id){
    return (getHost(id) != NULL);    
}

bool routerExists(std::string id){
    return (getRouter(id) != NULL);
}

bool linkExists(std::string id){
    return (getLink(id) !=NULL);
}

bool flowExists(std::string id){
    return (getFlow(id) != NULL);
}


// Add functions for all the classes
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
			node *n1 = nodes[node_id1];
			node *n2 = nodes[node_id2];
		
			links[id] = link(id, n1, n2, rate, delay, buffer);
		
			// Update relations
			n1->addLink(links[id]);
			n2->addLink(links[id]);

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
int net::addFlow(std::string id, std::node_src, std::node_dst, float data_size, float start_time, TCP_type tcp_enum){
	// Check if there exists a flow with this id already
	if (!flowExists(id)){
		// Check if both IDs are valid nodes
		if(nodeExists(node_src) && nodeExists(node_dst)){
			// Get the nodes
			src = nodes[node_src];
			dst = nodes[node_dst];
		
			//TODO: Implement TCP Algo
			flows[id] = flow(id, src, dst, data_size, start_time, tcp_enum);
			nflows++;
		
			// Update relations
			src.addFlow(flows[id]);
			dst.addFlow(flows[id]);
			
			// TODO: Create initial events
			event_start_flow *flowStart = new flowStart(start_time, *this, flows[id]);
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

// Decrement the number of active flows when one is done transmitting
int net::flowFinished(){
	return --nflows;
}

// Priority queue
int net::addEvent(event *e){
	e->setStart(e->getStart + time); // Adds global time to the initialized delay
	events.push(e);
}

// Runs the simulation
int net::run(){
	// Ends if there are no flows left
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
