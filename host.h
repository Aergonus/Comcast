#ifndef HOST_H
#define HOST_H

#include <vector>
#include <string>
#include "link.h"
#include "flow.h"
#include "packet.h"
#include "node.h"

using namespace std;

class host : public node {
	void rcv_pkt(packet * inc_packet);
	vector<int> pkts_rcvd;

public:
	host();

};
