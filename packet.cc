#include <string>

#include "packet.h"

//CONSTRUCTORS
packet::packet(node* origin, node* dest, int size, int seqNum, std::string data){
    this->origin = origin;
    this->dest = dest;
    this->size = size;
    this->seqNum = seqNum;
    this->data = data;
}
packet::packet(node* origin, node* dest, int size, int seqNum){
    packet(origin, dest, size, seqNum, "");
}

packet::packet(int size, int seqNum){
    packet(0, 0, size, seqNum);
}
packet::packet(int size){
    packet(0, 0, size, 0);
}

//FUNCTIONS
//returns the origin node pointer
node* packet::getOrigin(){
     return origin;
}

//returns the destination node pointer
node* packet::getDest(){
    return dest;
        }

//returns the size of the packet in bits
int packet::getSize(){
    return size;
        }

//returns the sequenence number of the packet
int packet::getSeqNum(){
    return seqNum;
        }

//returns the data inside the packet(whatever it is)
std::string packet::getData(){
    return data;
    }