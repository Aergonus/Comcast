/**
 * ECE408 
 * packet.h
 * Purpose: 
 * 
 * @author EuiSeong Han, Eric Nguyen, Kangqiao Lei
 * @version 0.1.5 04/05/16
 */

#ifndef PACKET_H
#define PACKET_H

#include "util.h"

class node;

class packet{
    private:
        node *src;//sender of the packet(same as the sender of the flow)
        node *dst;//dstination of the packet (same as the dstination of the flow)
		packet_type type; // ENUM
		int size; //size of the packet. Unit: bits
		int ks;
    public:
        //constructors
        packet(node *src, node *dst, packet_type type, int killswitch)...
			:src(src), dst(dst), type(type), size(size), ks(killswitch);
		
        //FUNCTIONS
        //returns the src node pointer
        node* getSrc(){return src;};
        //returns the dstination node pointer
        node* getDst(){return dst;};
        //returns the size of the packet in bits
        int getSize(){return size;};
		//returns the ks number of the packet
		int getKillSwitch(){return ks;};
		//decrement Kill Switch
		int perishSong(){return --ks;};
};

class data_pak : public packet{
	private:
		int seqNum; //The sequence number of the packet.
		int ackNum; //The acknowledgement number of the packet. 
		flow *pFlow;//parent flow 
		
	public: 
		data_pak(node *src, node *dst, packet_type type, int killswitch, ...
			int size, int seqNum, int ackNum, flow *f) : packet(src, dst, type, killswitch), pFlow(f), seqNum(seqNum), ackNum(ackNum);
		
		flow* getFlow(){return pFlow;};
		//returns the sequence number of the packet
        int getSeqNum(){return seqNum;};
        //returns the ack number of the packet
        int getAckNum(){return ackNum;};
}

//TODO: Routing Packet
#endif