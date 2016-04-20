/**
 * ECE408 
 * event_start_flow.h
 * Purpose: Initializes flow for packet sending
 * 
 * @author Kangqiao Lei
 * @version 0.2.0 04/19/16
 */

#ifndef EVENT_START_FLOW_H
#define EVENT_START_FLOW_H

#include "event.h"
#include "link.h"

// Forward Declaration
class net;

class event_start_flow : public event {
	private:
	link *f;
	
	public:
	event(float time, net *Network, link *l):start(time), sim(Network), l(l);
	
	void handle_event(){
		f->start_flow();
	};
};
#endif