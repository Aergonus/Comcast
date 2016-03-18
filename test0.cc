#include "flow.h"
//#include "node.h"
#include "link.h"
//#include "packet.h"
#include "host.h"

int main(){
	host* host1 = new host("host1", 1);
	host* host2 = new host("host2", 2);
	link* l1 = new link(1, 0.001, 10000, host1, host2);
	flow* f1 = new flow("f1", host1, host2, 10);
	host1->addFlow(f1);
	host1->addLink(l1);
	host1->sendPak();

	for (float time=0; time < 1000;){
		if(! l1->empty()){
			time += (l1->current_delay());
			l1->transmit_pkt();
			std::cout<<time<<"seconds"<<std::endl;
		}
		else{
			std::cout<<"simulation ended"<<std::endl;
			break;
		}
	}
	return 0;
}