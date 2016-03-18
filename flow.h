#ifndef FLOW_H
#define FLOW_H

#include <string>
#include <vector>
#include <iterator>
//#include "host.h"
#include "packet.h"
class host;
class flow{
    friend class packet;    
    public:
        //Constructors
        /*flow(std::string id, node* origin, node* dest, int flowSize);
        flow(node* origin, node* dest, int flowSize);
        flow(std::vector<packet*> data);*/
        flow(std::string id, node* origin, node* dest, int flowSize){
            myID = id;
            myOrigin = origin;
            myDest = dest;
            mySize = flowSize;
        }
        flow(node* origin, node* dest, int flowSize): myID("0000"), myOrigin(origin),myDest(dest),mySize(flowSize){
        }
        flow(std::vector<packet> data): myID("0000"), myOrigin(data[0].getOrigin()), myDest(data[0].getDest()), mySize(data.size()*(data[0].getSize())) {
        }
         //function to obtain the origin of the flow
        node* getOrigin(){
            return myOrigin;
        }
        //function to obtain the destination of the flow
        node* getDest(){
            return myDest;
        }
        //function to obtain the id of the flow
        std::string getId(){
            return myID;
        }
        //function to obtain the size of the flow
        int getSize();
        //function to obtain the number of packets in the flow
        //function to obtain the number of packets in the flow
        int numPack(){
            return packetList.size();
        }
        //function to obtain ith packet in the flow
        packet* getPacket(int i){
            return packetList[i];
        }
        
    private:
        node* myOrigin;
        node* myDest;
        std::string myID;
        int mySize; //Total size of the flow. Unit: bit
        std::vector<packet*> packetList; //List of the packets that comprise the flow
        void packup(); // when only the flow size is given (not the actual list of packets), divide the entire data into packets and store them as a vector "packetList"   
};
#endif