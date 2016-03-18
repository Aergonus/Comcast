#include "flow.h"
#include "host.h"
#include "link.h"

int main(){
	host* host1 = new host("HOST1", 1);
	host* host2 = new host("HOST2", 2);
	host* host3 = new host("HOST3", 3);

	link* l1 = new link(10, 0.001, 10000, host1, host2);

	flow* f1 = new flow("f1", host1, host2, 10);

	host1->addFlow(f1);
	host1->addLink(l1);
	host2->addLink(l1);
	host1->sendPak();

	//SIMULATION
	for (float time=0; time < 1000;){
		if(! l1->empty()){
			time += (l1->current_delay());
			std::cout<<time<<" seconds ::  ";
			l1->transmit_pkt();
		}
		else{
			std::cout<<std::endl<<"SIMULATION ENDED"<<std::endl;
			break;
		}
	}
	return 0;
}