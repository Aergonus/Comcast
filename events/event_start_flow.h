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
#include "../flow.h"

class event_start_flow : public event {
	private:
	Flow *f;
	
	public:
	event_start_flow(float time, Flow *f):event(time), f(f) {};
	~event_start_flow(){};
	
	void handle_event(){
		f->start_Flow();
	};
	
	void print() {
		*debugSS << "Flow Event at time " << get_Start() << " currently " << isValid() << std::endl;
	}
};
#endif