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

#include "flow.h" // DEBUG ONLY 
#include "util.h"

class Node;
class Flow; 

class Packet{
	private:
		Node *src;//sender of the Packet(same as the sender of the Flow)
		Node *dst;//dstination of the Packet (same as the dstination of the Flow)
		int size; //size of the Packet. Unit: bits
		int ks;
	public:
		packet_type type; // ENUM
		
		//constructors
		Packet(Node *src, Node *dst, int size, int killswitch, packet_type type)
			:src(src), dst(dst), size(size), ks(killswitch), type(type) {};
		virtual ~Packet(){};
		
		//FUNCTIONS
		virtual Flow* getFlow() = 0;
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
		
		virtual int getSeqNum() = 0;
		virtual int getAckNum() = 0;
		// Print Packet details
		virtual void print() = 0;
};

class data_pak : public Packet{
	private:
		int seqNum; //The sequence number of the Packet.
		Flow *pFlow;//parent Flow 
		
	public: 
		data_pak(Node *src, Node *dst, int size, int killswitch, packet_type type, int seqNum, Flow *f) 
			: Packet(src, dst, size, killswitch, type), seqNum(seqNum), pFlow(f) {};
		~data_pak(){};
		
		Flow* getFlow(){return pFlow;};
		//returns the sequence number of the Packet
		int getSeqNum(){return seqNum;};
		//returns the ack number of the Packet
		int getAckNum(){return seqNum+getSize();};
		void print(){
			*debugSS << "Data packet for flow " << pFlow->getName() << ". SeqNum " << getSeqNum() << ". AckNum " << getAckNum() << ". Flow size: " << pFlow->getSize() << std::endl;
		};
};

class ack_pak : public Packet{
	private:
		int ackNum; //The acknowledgement number of the Packet. 
		Flow *pFlow;//parent Flow 
		
	public: 
		ack_pak(Node *src, Node *dst, int size, int killswitch, packet_type type, int ackNum, Flow *f) 
			: Packet(src, dst, size, killswitch, type), ackNum(ackNum), pFlow(f) {};
		~ack_pak(){};
		
		Flow* getFlow(){return pFlow;};
		int getSeqNum(){return -1;};
		//returns the ack number of the Packet
		int getAckNum(){return ackNum;};
		void print(){
			*debugSS << "Ack packet for flow " << pFlow->getName() << ". SeqNum " << getSeqNum() << ". AckNum " << getAckNum() << ". Flow size: " << pFlow->getSize() << std::endl;
		};
};
/*
class rout_pak : public Packet{
	public: 
		std::map<std::string, std::map<std::string, float> > routing_table;
		
		rout_pak(Node *src, Node *dst, packet_type type, int size, int killswitch, std::map<std::string, std::map<std::string, float> > rt) 
			: Packet(src, dst, type, size, killswitch), rt(routing_table) {};
};
*/
#endif
