/**
 * ECE408 
 * event_TO.h
 * Purpose: Timeout Event for Flow
 * 
 * @author Kangqiao Lei
 * @version 0.2.0 04/21/16
 */

#ifndef EVENT_TO_H
#define EVENT_TO_H

#include "event.h"
#include "../flow.h"

class event_TO : public event {
	private:
	float timeout_val;
	Flow *f;
	
	public:
	event_TO(float time, Flow *f):event(time), timeout_val(time), f(f) {};
	~event_TO(){};
	
	float getTO(){return timeout_val;}
	
	void handle_event(){
		f->timeout_Flow();
	}
	
	void print() {
		*debugSS << "Starting TO Event at time " << get_Start() << " currently " << isValid() << std::endl;
	}
};
#endif
