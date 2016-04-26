#include "network.h"

void Network::AddHost(const std::string id){
  devices_.push_back(Host(id));
}

void Network::AddRouter(const std::string id){
  devices_.push_back(Router(id));
}

void Network::AddLink(const std::string id, const std::string end1_id, const std::string end2_id,
                      double datarate, double delay, double buffer){

  Node& end1 = FindDevice(end1_id);
  Node& end2 = FindDevice(end2_id);

  links_.push_back(Link(id, end1, end2, datarate, delay, buffer));
}

void Network::AddFlow(const std::string id, double start_time, int size,
                      const std::string src_id, const std::string dst_id, 
                      std::string protocol){

  Node& src = FindDevice(src_id);
  Node& dst = FindDevice(dst_id);
  flows_.push_back( Flow(id, start_time, size, dynamic_cast<Host&>(src), dynamic_cast<Host&>(dst), protocol) );
}

std::vector<Flow>& Network::GetFlows(){
  return flows_;
}

std::vector<Link>& Network::GetLinks(){
  return links_;
}

std::vector<Node>& Network::GetDevices(){
  return devices_;
}

Node& Network::FindDevice(std::string id){
  for(std::vector<Node>::iterator itr = devices_.begin();
      itr != devices_.end(); ++itr){
    if (itr->GetId() == id) {return *itr;}
  }
}
