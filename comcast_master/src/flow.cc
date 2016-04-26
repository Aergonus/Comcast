#include "flow.h"
#include "packet.h"
#include "host.h"
#include "event_manager.h"

Flow::Flow(const std::string id, double start_time, int size, const Host& src, 
           const Host& dst, const std::string protocol):
  id_(id),
  start_time_(start_time),
  size_(size),
  src_(src),
  dst_(dst),
  protocol_(protocol){}



const double Flow::GetStartTime() const{
  return start_time_;
}

void Flow::Pack(){
  int data = size_;
  int i=1;
  while (data!=0){
    packets_.push_back(Packet("D",i,src_, dst_));
    ++i;
    data -= universe::PACKET_SIZE;
  }
  num_packs_ = packets_.size();
}

void Flow::Start(){
  const std::vector<Packet>::iterator itr = packets_.begin();
  if (itr == packets_.end()){
    //event_manager.push(FlowEndEvent(this, event_manager.global_time());
  }

  else if (src_.allowedToTransmit()){
    //event_manager.push(SendPacketEvent(*itr, event_manager.global_time()));
  }
}
