

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "link.h"
#include "flow.h"
#include "packet.h"

using namespace std;

class node {
    node * id
    vector<link *> links;
    
public:
    node();
    node * get_id();
    void add_link(link * new_link);
    vector<link *> get_links();
    link * get_link(node * destination);
    void send_pkt(packet * out_packet, link * out_link);
    void rcv_pkt(packet * inc_packet);
};

#endif
    
