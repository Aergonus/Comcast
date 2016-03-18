#ifndef PACKET_H
#define PACKET_H

#include <string>
//#include "node.h"
class node;
class packet{
    private:
        node* origin;//sender of the packet(same as the sender of the flow)
        node* dest;//destination of the packet (same as the destination of the flow)
        int size; //size of the packet. Unit: bits
        int seqNum; //The sequence number of the packet.
        std::string data; //The actual data packet carries (I don't know if we need this for the simulation but I am including it for completeness.
    public:
        //constructors
        packet(node* origin, node* dest, int size, int seqNum, std::string data);
        packet(node* origin, node* dest, int size, int seqNum);
        packet(int size, int seqNum);
        packet(int size);

        node* getOrigin(); //returns the sender info
        node* getDest(); //returns the destination info
        int getSize(); //returns the size of the packet
        int getSeqNum(); //returns the sequence number of the packet
        std::string getData(); //returns the data inside the packet (whatever it is)
};
#endif