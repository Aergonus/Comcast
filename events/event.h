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
	bool invaldiate(){return valid = false;}
	float getStart(){return start;};
	float setStart(float stime){return start = stime;};
};

// Sorting rule for the event* priority queue 
struct compareEvents {
  bool operator() (Event * eventA, Event * eventB) {
    return eventA->get_start() > eventB->get_start();
  }
};
#endif