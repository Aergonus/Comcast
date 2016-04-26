#ifndef HOST_H_
#define HOST_H_

#include "node.h"
#include "event.h"
#include "event_manager.h"

class Host: public Node{
  public:
    Host(const std::string id);
    Event SendPacket(const Packet &p, const double time);
    Event RecievePacket(const Packet &p, const double time);
    const bool allowedToTransmit() const;
};
#endif
