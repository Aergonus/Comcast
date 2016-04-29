/**
 * ECE408 
 * event.h
 * Purpose: Barebones Base Event Object
 * 
 * @author Kangqiao Lei
 * @version 0.2.0 04/19/16
 */

#ifndef EVENT_H
#define EVENT_H

#include <functional>

class event {
	private:
	float start;
	bool valid;
	
	public:
	event(float stime):start(stime) {
		valid = true;
	} ;
	
	virtual void handle_event();
	bool isValid(){return valid;};
	bool invalidate(){return valid = false;};
	float get_Start(){return start;};
	float set_Start(float stime){return start = stime;};
};

// Sorting rule for the event* priority queue 
struct compareEvents {
  bool operator() (event *eventA, event *eventB) const {
	return eventA->get_Start() > eventB->get_Start();
  }
};
#endif