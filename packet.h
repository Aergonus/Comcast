#ifndef PACKET_H
#define PACKET_H

#include <string>

class node;
class packet{
    private:
        node* origin;//sender of the packet(same as the sender of the flow)
        node* dest;//destination of the packet (same as the destination of the flow)
        int size; //size of the packet. Unit: bits
        int seqNum; //The sequence number of the packet.
        std::string data; //The actual data packet carries (I don't know if we need this for the simulation but I am including it for completeness.
    public:
        //CONSTRUCTORS
        packet(node* origin, node* dest, int size, int seqNum, std::string data);
        packet(node* origin, node* dest, int size, int seqNum);
        packet(int size, int seqNum);
        packet(int size);

        //FUNCTIONS
        //returns the origin node pointer
        node* getOrigin();
        //returns the destination node pointer
        node* getDest();
        //returns the size of the packet in bits
        int getSize();
        //returns the sequence number of the packet
        int getSeqNum();
        //returns the data inside the packet (whatever it is)
        std::string getData();
};
#endif