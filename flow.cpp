/**
 * ECE408 
 * flow.cpp
 * Purpose: 
 * 
 * @author EuiSeong Han, Eric Nguyen, Kangqiao Lei
 * @version 0.1.5 04/05/16
 */
 
#include <string>
#include <vector>
#include <iterator>
#include "node.h"
#include "packet.h"
class flow{
    friend class packet;
    
    private:
    node* myOrigin;
    node* myDest;
    string myId;
    int size; 
    std::vector<packet> packetList; //List of the packets that comprise the flow

    public:
    //Constructors
    flow(std::string id, node* origin, node* dest, int flowSize){
        myId = id;
        myOrigin = origin;
        myDest = dest;
        mySize = flowSize;
    }
    flow(node* origin, node* dest, int flowSize){
        flow( "0000", origin, dest, flowSize);   
    }
    flow(std::vector<packet> data){
        flow ( "0000", data[0].getOrigin, data[0].getDest, data.size()*data[0].packetSize());
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
    string getId(){
        return myId
    }
    //function to obtain the size of the flow
    int getSize(){
        return mySize;
    }
    //function to obtain the number of packets in the flow
    int numPacks(){
        return packetList.size();
    }
    packet& getPacket(int i){
        return packetList[i];
    }
    private void packUp(){
        int tmp = mySize;
        int i =0;
        while (tmp>0){
            packetList.add(new packet(i));
            tmp = tmp - packet::defaultSize;
            ++i;
        }
    }
}














