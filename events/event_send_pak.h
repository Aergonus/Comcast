/**
 * ECE408 
 * event_send_pak.h
 * Purpose: Sent Packet Event 
 * 
 * @author Kangqiao Lei
 * @version 0.5.0 05/03/16
 */

#ifndef EVENT_SEND_PAK_H
#define EVENT_SEND_PAK_H

#include "event.h"
#include "../Link.h"

class event_send_pak:public event{
	private:
	Link *l;
	
	public:
	event_send_pak(float time, Link *l):event(time), l(l){
		setType("Send Packet Event");
	};
	~event_send_pak(){};
	
	void handle_event(){
		l->send_pak();
	};
};
#endif