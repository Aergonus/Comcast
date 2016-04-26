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
#include "flow.h"

class event_start_flow : public event {
	private:
	flow *f;
	
	public:
	event_start_flow(float time, flow *f):event(time), f(f);
	
	void handle_event(){
		f->start_flow();
	};
};
#endif