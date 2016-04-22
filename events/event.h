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

// Forward Declaration
class net;

class event {
	private:
	float start;
	net* sim;
	bool valid;
	
	public:
	event(float time, net* Network):start(time), sim(Network) {
		valid = true;
	} ;
	
	virtual void handle_event();
	float getStart(){return start;};
	float setStart(float delay){start = delay;};
};

// Sorting rule for the event* priority queue 
struct compareEvents {
  bool operator() (Event * eventA, Event * eventB) {
    return eventA->get_start() > eventB->get_start();
  }
};
#endif