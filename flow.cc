#include <string>
#include <vector>
#include <iterator>
//#include "node.h"
//#include "packet.h"
#include "flow.h"
class flow{
    friend class packet;
    private:
        int mySize; //Total size of the flow. Unit: bit
        std::vector<packet*> packetList; //List of the packets that comprise the flow
    public:
        //Constructors
        /*flow(std::string id, node* origin, node* dest, int flowSize){
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
        }*/
       


        //function to obtain the size of the flow
        int getSize(){
            return mySize;
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
};














