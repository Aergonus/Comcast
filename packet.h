/**
 * ECE408 
 * Packet.h
 * Purpose: Packet Data Definitions
 * 
 * @author EuiSeong Han, Eric Nguyen, Kangqiao Lei
 * @version 0.5.0 05/03/16
 */

#ifndef PACKET_H
#define PACKET_H

// Required Libraries
#include <map> // Needed for Control Packets
#include "flow.h" // Data/Ack Debug Print Only
#include "router.h" // Crtl Debug Print Only
#include "util.h"

// Forward Declarations
class Node;
class Flow;

class Packet{
	private:
		Node *src; // sender of the Packet(same as the sender of the Flow)
		Node *dst; // destination of the Packet (same as the destination of the Flow)
		int size; // size of the Packet in bytes
		int ks;
		float createTime;
	public:
		packet_type type; // ENUM
		
		// constructors
		Packet(Node *src, Node *dst, int size, int killswitch, packet_type type)
			:src(src), dst(dst), size(size), ks(killswitch), type(type){
			createTime = simtime;};
		virtual ~Packet(){};
		
		// FUNCTIONS
		virtual Flow* getFlow() = 0;
		// returns the src Node pointer
		Node* getSrc(){return src;};
		// returns the dstination Node pointer
		Node* getDst(){return dst;};
		// returns the size of the Packet in bytes
		int getSize(){return size;};
		// returns the ks number of the Packet
		int getKillSwitch(){return ks;};
		// decrement Kill Switch
		int perishSong(){return --ks;};
		// returns create time of Packet
		float getCT(){return createTime;};
		// returns the type enum
		packet_type getType(){return type;};
		
		virtual int getSeqNum() = 0;
		virtual int getAckNum() = 0;
		virtual std::map<std::string, float>* getuDVT() = 0;
		virtual std::map<std::string, std::map<std::string, float> >* getRT() = 0;

		// Print Packet details
		virtual void print() = 0;
		virtual void printERR() = 0;
};

class data_pak:public Packet{
	private:
		int seqNum; // The sequence number of the Packet.
		Flow *pFlow; // parent Flow 
		
	public:
		data_pak(Node *src, Node *dst, int size, int killswitch, packet_type type, int seqNum, Flow *f) 
			:Packet(src, dst, size, killswitch, type), seqNum(seqNum), pFlow(f){};
		~data_pak(){};
		
		Flow* getFlow(){return pFlow;};
		int getSeqNum(){return seqNum;}; // returns the sequence number of the Packet
		int getAckNum(){return seqNum+getSize();}; // returns the ack number of the Packet
		std::map<std::string, float>* getuDVT(){return NULL;};
		std::map<std::string, std::map<std::string, float> >* getRT(){return NULL;};
		
		void print(){
			*debugSS<<"Data packet for flow "<<pFlow->getName()<<",SeqNum,"<<getSeqNum()<<",AckNum,"<<getAckNum()<<",Flow size,"<<pFlow->getSize()<<std::endl;
		};
		void printERR(){
			*errorSS<<"Data packet for flow "<<pFlow->getName()<<",SeqNum,"<<getSeqNum()<<",AckNum,"<<getAckNum()<<",Flow size,"<<pFlow->getSize()<<std::endl;
		};
};

class ack_pak:public Packet{
	private:
		int ackNum; // The acknowledgement number of the Packet. 
		Flow *pFlow; // parent Flow 
		
	public:
		ack_pak(Node *src, Node *dst, int size, int killswitch, packet_type type, int ackNum, Flow *f) 
			:Packet(src, dst, size, killswitch, type), ackNum(ackNum), pFlow(f){};
		~ack_pak(){};
		
		Flow* getFlow(){return pFlow;};
		int getSeqNum(){return -1;}; // Flag for ack 
		int getAckNum(){return ackNum;}; // returns the ack number of the Packet
		std::map<std::string, float>* getuDVT(){return NULL;};
		std::map<std::string, std::map<std::string, float> >* getRT(){return NULL;};
		
		void print(){
			*debugSS<<"Ack packet for flow "<<pFlow->getName()<<",SeqNum,"<<getSeqNum()<<",AckNum,"<<getAckNum()<<",Flow size,"<<pFlow->getSize()<<std::endl;
		};
		void printERR(){
			*errorSS<<"Ack packet for flow "<<pFlow->getName()<<",SeqNum,"<<getSeqNum()<<",AckNum,"<<getAckNum()<<",Flow size,"<<pFlow->getSize()<<std::endl;
		};
};

class crtl_pak:public Packet{
	private:
		std::map<std::string, std::map<std::string, float> > routing_table; // Snapshot of source routing table
		std::map<std::string, float> updatedDVTime; // Snapshot of source updatedDVTime vector
	
	public:
		crtl_pak(Node *src, Node *dst, int size, int killswitch, packet_type type, std::map<std::string, std::map<std::string, float> > rt, std::map<std::string, float> uDVT)
			:Packet(src, dst, size, killswitch, type), routing_table(rt), updatedDVTime(uDVT){}
		~crtl_pak(){};

		Flow* getFlow(){return NULL;};
		int getSeqNum(){return 0;};
		int getAckNum(){return 0;};
		std::map<std::string, float>* getuDVT(){return &updatedDVTime;};
		std::map<std::string, std::map<std::string, float> >* getRT(){return &routing_table;};
		
		void print(){
			*debugSS<<"Crtl packet for Router "<<getSrc()->getName()<<std::endl;
		};
		void printERR(){
			*errorSS<<"Crtl packet for Router "<<getSrc()->getName()<<std::endl;
		};
};

#endif