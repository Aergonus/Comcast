#include <iostream>
#include "event_manager.h"
#include "event.h"
#include "network.h"
#include "flow.h"
#include "host.h"
#include "router.h"
#include "link.h"
#include "global.h"

EventManager::EventManager(std::string outfile, Network &net) : 
  flows_(net.GetFlows()), links_(net.GetLinks()), devices_(net.GetDevices()){
  output_filename_ = outfile;
  global_time_ = 0;
}

void EventManager::log(const std::string msg){
  out_file_.open(output_filename_ + ".log");
  out_file_ << msg <<std::endl;
  out_file_.close();
}

void EventManager::Setup(){
  for (std::vector<Flow>::iterator itr = flows_.begin(); 
        itr != flows_.end(); ++itr){
      itr->Pack();
      queue_.push(FlowStartEvent(*itr, itr->GetStartTime()));
  }
}

double EventManager::global_time() const{
  return global_time_;
}

const bool EventManager::isDone() const{
  if (finished_ >= flows_.size()) {return true;}
}

void EventManager::Run(){ 
  while( true ){
    if (global_time_ >= MAX_SIMULATION_TIME)  {break;}
    if (isDone()) {break;}
    Event& e = const_cast<Event&>(queue_.top());
    queue_.pop();
    global_time_ = e.GetScheduledTime();
    e.Start();
  }
}
void EventManager::push(Event e){
  queue_.push(e); 
}


