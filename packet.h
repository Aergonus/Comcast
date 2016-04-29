/**
 * ECE408 
 * Packet.h
 * Purpose: 
 * 
 * @author EuiSeong Han, Eric Nguyen, Kangqiao Lei
 * @version 0.2.0 04/19/16
 */

#ifndef PACKET_H
#define PACKET_H

#include "util.h"

class Node;
class Flow; 

class Packet{
	private:
		Node *src;//sender of the Packet(same as the sender of the Flow)
		Node *dst;//dstination of the Packet (same as the dstination of the Flow)
		packet_type type; // ENUM
		int size; //size of the Packet. Unit: bits
		int ks;
	public:
		//constructors
		Packet(Node *src, Node *dst, packet_type type, int size, int killswitch)
			:src(src), dst(dst), type(type), size(size), ks(killswitch) {};
		
		//FUNCTIONS
		//returns the src Node pointer
		Node* getSrc(){return src;};
		//returns the dstination Node pointer
		Node* getDst(){return dst;};
		//returns the size of the Packet in bits
		int getSize(){return size;};
		//returns the ks number of the Packet
		int getKillSwitch(){return ks;};
		//decrement Kill Switch
		int perishSong(){return --ks;};
	// Print Packet details
	std::string print();
};

class data_pak : public Packet{
	private:
		int seqNum; //The sequence number of the Packet.
		Flow *pFlow;//parent Flow 
		
	public: 
		data_pak(Node *src, Node *dst, packet_type type, int size, int killswitch, int seqNum, Flow *f) 
			: Packet(src, dst, type, size, killswitch), seqNum(seqNum), pFlow(f) {};
		
		Flow* getFlow(){return pFlow;};
		//returns the sequence number of the Packet
		int getSeqNum(){return seqNum;};
		//returns the ack number of the Packet
		int getAckNum(){return seqNum+getSize();};
};

class ack_pak : public Packet{
	private:
		int ackNum; //The acknowledgement number of the Packet. 
		Flow *pFlow;//parent Flow 
		
	public: 
		ack_pak(Node *src, Node *dst, packet_type type, int size, int killswitch, int ackNum, Flow *f) 
			: Packet(src, dst, type, size, killswitch), ackNum(ackNum), pFlow(f) {};
		
		Flow* getFlow(){return pFlow;};
		//returns the ack number of the Packet
		int getAckNum(){return ackNum;};
};

//TODO: Routing Packet
#endif
