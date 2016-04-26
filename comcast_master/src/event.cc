#include "event.h"
#include "packet.h"
#include "event_manager.h"

bool Event::operator < (const Event& e) const{
  return this.schedule_at_ < e.schedule_at_;
}
bool Event::operator > (const Event& e) const{
  return this.schedule_at_ > e.schedule_at_;
}
bool Event::operator == (const Event& e) const{
  return this.schedule_at_ == e.schedule_at_;
}

FlowStartEvent::FlowStartEvent(const Flow &f, const double t) 
  : flow_to_start_(f), schedule_at_(t){}

double Event::GetScheduledTime(){
  return schedule_at_;
}

void FlowStartEvent::Start(){
  flow_to_start_.Start();
}

FlowEndEvent::FlowEndEvent(const Flow &f, const double t)
  :flow_to_end_ (f), schedule_at_(t){}


void FlowEndEvent::Start(){
  flow_to_end_.End();
}

SendPacketEvent::SendPacketEvent(const Packet &p, const node& src, const node& dst, const double t)
  :packet_to_send_(p), schedule_at_(t){}


void SendPacketEvent::Start(){
  packet_to_send_.GetSrc().SendPacket(packet_to_send_, schedule_at_);
}

ReceivePacketEvent::ReceivePacketEvent(const Packet &p, const double t)
  :packet_to_receive_(p), schedule_at_(t){}


void ReceivePacketEvent::Start(){
  packet_to_receive_.GetDst().ReceivePacket(packet_to_receive_, schedule_at_);
}

