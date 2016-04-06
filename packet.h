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

class node;

class packet{
    private:
        node *src;//sender of the packet(same as the sender of the flow)
        node *dst;//dstination of the packet (same as the dstination of the flow)
        flow *pFlow;//parent flow 
		int size; //size of the packet. Unit: bits
        int seqNum; //The sequence number of the packet.
		int ackNum; //The acknowledgement number of the packet. 
		int killswitch;
    public:
        //constructors
        packet(node *src, node *dst, flow *f, int size, int seqNum, int ackNum, int ks)...
			:src(src), dst(dst), pFlow(f), size(size), seqNum(seqNum), ackNum(ackNum), killswitch(killswitch);
		
        //FUNCTIONS
        //returns the src node pointer
        node* getSrc(){return src;};
        //returns the dstination node pointer
        node* getDst(){return dst;};
		flow* getFlow(){return pFlow;};
        //returns the size of the packet in bits
        int getSize(){return size;};
        //returns the sequence number of the packet
        int getSeqNum(){return seqNum;};
        //returns the ack number of the packet
        int getAckNum(){return ackNum;};
		//returns the ks number of the packet
		int getKillSwitch(){return killswitch;};
		//decrement Kill Switch
		int perishSong(){return --killswitch;};
};
#endif
