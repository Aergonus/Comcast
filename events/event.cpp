/**
 * ECE408 
 * event.cpp
 * Purpose: Barebones Base Event Object
 * 
 * @author Kangqiao Lei
 * @version 0.1 04/01/16
 */

include "event.h"

event::event(float time, net* Network){
	start = time;
	sim = Network;
}
void Event::handle_event() {
	// Virtual Function - just a template; never called
}