#ifndef NETWORK_H_
#define NETWORK_H_

#include <vector>
#include "link.h"
#include "host.h"
#include "router.h"
#include "flow.h"
#include "global.h"

class Network{
  public:
    void AddHost(std::string id);
    void AddRouter(std::string id);
    void AddLink(std::string id, std::string end1_id, std::string end2_id, double datarate, double delay, double buffer);
    void AddFlow(std::string id, double start_time, int size, std::string src_id, std::string dst_id, std::string);
    std::vector<Flow>& GetFlows();
    std::vector<Link>& GetLinks();
    std::vector<Node>& GetDevices();
    Node& FindDevice(std::string);
  private:
    std::vector<Flow> flows_;
    std::vector<Link> links_;
    std::vector<Node> devices_;
};

#endif
