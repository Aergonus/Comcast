/**john's node.h
 * version x.x1
 */

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "link.h"
#include "flow.h"
#include "packet.h"

using namespace std;

class node {
    // Some form of addressing
    node * id
    // List of links connected to the node
    vector<link *> links;
    
public:
    // Constructor
    node();
    // Gets address of the node
    node * get_id();
    // Add link to the node
    void add_link(link * new_link);
    // Gets list of links connected to the node
    vector<link *> get_links();
    // Gets the link connected to the given node
    link * get_link(node * next_node);
    // Sends given packet through the given link
    void send_pkt(packet * out_packet, link * out_link);
};

#endif
    
