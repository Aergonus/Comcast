/**
 * ECE408 
 * event_TO.h
 * Purpose: Timeout Event for Flow
 * 
 * @author Kangqiao Lei
 * @version 0.5.0 05/03/16
 */

#ifndef EVENT_TO_H
#define EVENT_TO_H

#include "event.h"
#include "../flow.h"

class event_TO:public event{
	private:
	float timeout_val;
	Flow *f;
	
	public:
	event_TO(float time, Flow *f):event(time), timeout_val(time), f(f){
		setType("TO Event");
	};
	~event_TO(){};
	
	float getTO(){return timeout_val;}
	
	void print(){
		*debugSS<<getType()<<",EventID,"<<getID()<<",StartTime,"<<get_Start()<<",isValid,"<<isValid()<<",TO,"<<timeout_val<<std::endl;
	};
	
	void handle_event(){
		f->timeout_Flow();
	}
};
#endif