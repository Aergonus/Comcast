#include <vector>
#include <string>

//#include "flow.h"
//#include "node.h"
//#include "link.h"
//#include "packet.h"
//#include "util.h"
#include "host.h"
class host : public node {
	 public: 
		// The constructor creates the host,
		// which is initially unconnected and without flows.
		//host(std::string name,int address):node(name),address(address){
		//	this.name = name;
		//	this.address = address;
		//} // TODO: remove address and autoassign?
		
		// Initialize and link the host to the network as an endpoint

		// Create a link to a Router
		//int linkRouter(Router){}
		// Add a flow for the Host
		/*int addFlow(flow f){
			return flow.add(f);
		}*/
		// Sends a packet into the network via the host's link
		// Called by the Flow and the TLA

		

		
		// Debug print host name and address
		void print(){}
 };
 