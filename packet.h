/**
 * ECE408 
 * packet.h
 * Purpose: 
 * 
 * @author EuiSeong Han, Eric Nguyen, Kangqiao Lei
 * @version 0.2.0 04/19/16
 */

#ifndef PACKET_H
#define PACKET_H

<<<<<<< HEAD
#include "util.h"
class flow;
=======
>>>>>>> 8b583dc3049d36ee176e40937169c7f1d9ee4fe5
class node;
class flow;

class packet{
    protected:
        node *src;//sender of the packet(same as the sender of the flow)
        node *dst;//dstination of the packet (same as the dstination of the flow)
	packet_type type; // ENUM
<<<<<<< HEAD
	int size; //size of the packet. Unit: bits
	int ks;
        int seqNum;
	flow *pFlow;//parent flow 
    public:
        //constructors
        packet(node *src, node *dst, packet_type type, int killswitch, flow* f ...)
        	:src(src), dst(dst), type(type), size(size), ks(killswitch), pFlow(f){}
=======
	int ks;

    protected:	
        int size; //size of the packet. Unit: bits
    public:
        //constructors
        packet(node *src, node *dst, packet_type type, int killswitch): src(src), dst(dst), type(type), size(size), ks(killswitch){};
>>>>>>> 8b583dc3049d36ee176e40937169c7f1d9ee4fe5
		
        //FUNCTIONS
        //returns the src node pointer
        node* getSrc(){return src;}
        //returns the dstination node pointer
        node* getDst(){return dst;}
        //returns its flow
        flow* getFlow(){return pFlow;}
        //returns the size of the packet in bits
        int getSize(){return size;}
	//returns the ks number of the packet
	int getKillSwitch(){return ks;}
	//decrement Kill Switch
	int perishSong(){return --ks;}
	// Print packet details
	std::string print();
        packet_type getType(){return type;}
};
class ack_pak : public packet{
	private:
<<<<<<< HEAD
	  int ackNum; //The acknowledgement number of the packet. 
		
	public: 
	  ack_pak(node *src, node *dst, packet_type type, int killswitch, 
	                                 int size, int ackNum, flow *f, ...) 
            : packet(src, dst, type, killswitch, f), ackNum(ackNum){}
		
	  flow* getFlow(){return pFlow;}
        //returns the ack number of the packet
        int getAckNum(){return ackNum;}
=======
        	int seqNum; //The sequence number of the packet.
        	flow *pFlow;//parent flow 
		
	public: 
        	data_pak(node *src, node *dst, packet_type type, int killswitch, int size, int seqNum, flow *f) : packet(src, dst, type, killswitch), pFlow(f), seqNum(seqNum){};
		flow* getFlow(){return pFlow;};
		//returns the sequence number of the packet
        int getSeqNum(){return seqNum;};
        //returns the ack number of the packet
        int getAckNum(){return seqNum+size;};
>>>>>>> 8b583dc3049d36ee176e40937169c7f1d9ee4fe5
};


class data_pak : public packet{
	private:
<<<<<<< HEAD
	  int seqNum; //The sequence number of the packet.
		
        public: 
          data_pak(node *src, node *dst, packet_type type, int killswitch,
                                         int size, int seqNum, flow *f, ...) 
            : packet(src, dst, type, killswitch, f), seqNum(seqNum){}
	
	  flow* getFlow(){return pFlow;}
	  //returns the sequence number of the packet
          int getSeqNum(){return seqNum;}
          //returns the ack number of the packet
          int getAckNum(){return seqNum+size;}
};

//TODO: Routing Packet
=======
		int ackNum; //The acknowledgement number of the packet. 
		flow *pFlow;//parent flow 
		
	public: 
		data_pak(node *src, node *dst, packet_type type, int killswitch, int size, int ackNum, flow *f) : packet(src, dst, type, killswitch), pFlow(f), ackNum(ackNum){};
		flow* getFlow(){return pFlow;};
        //returns the ack number of the packet
        int getAckNum(){return ackNum;};
};

//TODO: Roting Packet
>>>>>>> 8b583dc3049d36ee176e40937169c7f1d9ee4fe5
#endif
