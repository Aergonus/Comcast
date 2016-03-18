#ifndef FLOW_H
#define FLOW_H

#include <string>
#include <vector>

#include "packet.h"

class host;
class flow{
    private:
        node* myOrigin;
        node* myDest;
        std::string myID;
        int mySize; //Total size of the flow. Unit: bit
        std::vector<packet*> packetList; //List of the packets that comprise the flow
        void packUp(); //divide mySize into packets and store in packetList     
    public:
        //CONSTRUCTORS
        flow(std::string id, node* origin, node* dest, int flowSize){
            myID = id;
            myOrigin = origin;
            myDest = dest;
            mySize = flowSize;
            for (int i = 1; i <= flowSize; i++){
                packet* p = new packet(origin,dest,1,i,"hoopblah");
                packetList.push_back(p);
            }
        }
        flow(node* origin, node* dest, int flowSize): myID("0000"), myOrigin(origin),myDest(dest),mySize(flowSize){
        }
        flow(std::vector<packet> data): myID("0000"), myOrigin(data[0].getOrigin()), myDest(data[0].getDest()), mySize(data.size()*(data[0].getSize())) {
        }

        //FUNCTIONS
        //returns origin node pointer of the flow
        node* getOrigin();
        //returns destination node pointer of the flow
        node* getDest();
        //returns id string of the flow
        std::string getId();
        //returns int number of bits of the flow
        int getSize();
        //returns number of packets in the flow
        int numPack();
        //function to obtain ith packet in the flow
        packet* getPacket(int i);

};
#endif