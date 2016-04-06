/**
 * ECE408 
 * event_send_pak.h
 * Purpose: Send Packet Event
 * 
 * @author Kangqiao Lei
 * @version 0.1.5 04/05/16
 */

#ifndef EVENT_SEND_PAK_H
#define EVENT_SEND_PAK_H

#include "event.h"
#include "link.h"

// Forward Declaration
class net;

// External Variables 
extern bool debug;
extern ostream &debugSS;
extern ostream &errorSS;

class event_send_pak : public event {
	private:
	link *l;
	
	public:
	event(float time, net *Network, link *l):start(time), sim(Network), l(l);
	
	void handle_event(){
		l->send_pak();
	};
};
