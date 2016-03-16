#include <string>
#include <vector>
#include <iterator>
#include "node.h"
#include "packet.h"
class flow{
    friend class packet;
    
    public:
    //Constructors
    flow(std::string id, node origin, node dest, int flowSize);
    flow(node origin, node dest, int flowSize);
    flow(std::vector<packet> data);
    //function to obtain the origin of the flow
    node getOrigin();
    //function to obtain the destination of the flow
    node getDest();
    //function to obtain the id of the flow
    string getID();
    //function to obtain the size of the flow
    int getSize();
    //function to obtain the number of packets in the flow
    int numPacks();
    //function to obtain ith packet in the flow
    packet& getPacket(int i);
    private:
    node myOrigin;
    node myDest;
    string myID;
    int flowSize; //Total size of the flow. Unit: bit
    std::vector<packet> packetList; //List of the packets that comprise the flow
    void packup(); // when only the flow size is given (not the actual list of packets), divide the entire data into packets and store them as a vector "packetList"   
}
