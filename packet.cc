#include <string>
//#include "node.h"
#include "packet.h"
class packet{
    public:
        packet(node* origin, node* dest, int size, int seqNum, std::string data){
            this.origin = origin;
            this.dest = dest;
            this.size = size;
            this.seqNum = seqNum;
            this.data = data;
        }
        packet(node* origin, node* dest, int size, int seqNum){
            packet(origin, dest, size, seqNum, "");
        }

        packet(int size, int seqNum){
            packet(null, null, size, seqNum);
        }
        packet(int size){
            packet(null, null, size, 0);
        }
        
        node* origin(){
            return origin;
        }
        node* dest(){
            return dest;
        }
        int size(){
            return size;
        }
        int seqNum(){
            return seqNum;
        }
        std::string data(){
            return data;
        }
};
