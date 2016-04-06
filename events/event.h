/**
 * ECE408 
 * event.h
 * Purpose: Barebones Base Event Object
 * 
 * @author Kangqiao Lei
 * @version 0.1.5 04/05/16
 */

#ifndef EVENT_H
#define EVENT_H

// Forward Declaration
class net;

// External Variables 
extern bool debug;
extern ostream &debugSS;
extern ostream &errorSS;

class event {
	private:
	float start;
	net* sim;
	
	public:
	event(float time, net* Network);
	
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