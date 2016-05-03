/**
 * ECE408 
 * event_init_RT.h
 * Purpose: Initializes Router Routing Tables
 * 
 * @author Kangqiao Lei
 * @version 0.5.0 05/03/16
 */

#ifndef EVENT_INIT_RT_H
#define EVENT_INIT_RT_H

#include "event.h"
#include "../net.h"

class event_init_rt : public event {
	private:
	net *Network;
	
	public:
	event_init_rt(float time, net *n):event(time), Network(n) {
		setType("Init RT Event");
	};
	~event_init_rt(){};
		
	void handle_event(){
		Network->init_Routing();
	};
};
#endif