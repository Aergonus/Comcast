/**
 * ECE408 
 * event.h
 * Purpose: Barebones Base Event Object
 * 
 * @author Kangqiao Lei
 * @version 0.1 04/01/16
 */

class net; // Forward Declaration

class event {
	private:
	float start;
	net* sim;
	
	public:
	event(float time, net* Network);
	
	virtual void handle_event();
	float getStart(){return start;};
	float setStart(float delay){start = delay;};
}

// Sorting rule for the event* priority queue 
struct compareEvents {
  bool operator() (Event * eventA, Event * eventB) {
    return eventA->get_start() > eventB->get_start();
  }
};
