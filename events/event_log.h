/**
 * ECE408 
 * event_Log.h
 * Purpose: Logging Event for Throughput
 * 
 * @author Kangqiao Lei
 * @version 0.2.0 04/21/16
 */

#ifndef EVENT_LOG_H
#define EVENT_LOG_H

#include "event.h"
#include "../net.h"

class event_log : public event {
	private:
	float last_updated;
	net *Network;
	
	public:
	event_log(float time, net *n):event(time), Network(n) {
		setType("LoggingEvent");
		last_updated = simtime;
	};
	~event_log(){};
	
	void print(){
		*debugSS << getType() << ",EventID," << getID() <<",StartTime," << get_Start() << ",isValid," << isValid() << ",lastUpdated," << last_updated << std::endl;
	};
	
	void handle_event(){
		Network->log_Throughput();
	}
};
#endif
