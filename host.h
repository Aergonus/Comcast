/**john's host.h
 * version x.x1
 */

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
	// Receives incoming packet
	void rcv_pkt(packet * inc_packet);
	// Store list of received packets
	vector<int> pkts_rcvd;

public:
	host();

};

#endif
