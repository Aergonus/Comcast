#include <string>
#include <vector>
#include <iostream>

#include "flow.h"

//function to obtain the origin of the flow
node* flow::getOrigin(){
    return myOrigin;
}

//returns destination node pointer of the flow
node* flow::getDest(){
    return myDest;
}

//returns id string of the flow
std::string flow::getId(){
    return myID;
}

//returns int number of bits of the flow
int flow::getSize(){
    return mySize;
}

//returns number of packets of the flow
int flow::numPack(){
    return packetList.size();
}

//returns ith packet pointer of the flow
packet* flow::getPacket(int i){
    return packetList[i];
}

void flow::packUp(){
    int tmp = mySize;
    int i =0;
    while (tmp>0){
        packetList.push_back(new packet(i));
        //tmp = tmp - packet::getSize();
        ++i;
        std::cout <<"I KNOW HOW TO WRITE AN ERROR MESSAGE" << std::endl; 
        break;
    }
};
