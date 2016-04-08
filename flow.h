/**
 * ECE408 
 * flow.h
 * Purpose: 
 * 
 * @author EuiSeong Han, Eric Nguyen, Kangqiao Lei
 * @version 0.1.5 04/05/16
 */

#ifndef FLOW_H
#define FLOW_H

#include <string>

class node;

class flow {
	friend class packet;
	
	private:
        node* src;
        node* dst;
        std::string name;
        int size;
		float start;
		
		int seq, ack, expectedSeq, expectedAck, acksReceived, outstandingPaks;
    public:
    //Constructors
    flow(std::string id, node src, node dst, int flowSize, float startTime)...
	: name(id), src(src), dst(dst), size(flowSize), start(startTime);

    //function to obtain the origin of the flow
    node* getSrc(){return src;};
    //function to obtain the destination of the flow
    node* getDst(){return dst;};
    //function to obtain the id of the flow
    string getID(){return id;};
    //function to obtain the size of the flow
    int getSize(){return size;};
	
	void start_Flow();
	packet* send_Pak();
	packet* recieve_Pak();
	void nolove();
	
};
#endif