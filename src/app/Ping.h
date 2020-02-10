//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef APP_PONG_H_
#define APP_PONG_H_

#include <omnetpp/csimplemodule.h>
#include "inet/common/INETDefs.h"
#include "inet/common/packet/Packet.h"
#include "../msg/PingPong_m.h"

class Ping: public omnetpp::cSimpleModule {
protected:
  omnetpp::cMessage* timer;
  inet::PingPongPacket* rep;
  omnetpp::simtime_t ping_delay;
  enum status {INFO=0, NO_INFO};
  status node_status;
  int input_gate_id, output_gate_id, msg_counter, packet_size, seq;
protected:
  virtual void send_packet( );
  virtual void process_pong(omnetpp::cMessage*);
public:
    Ping();
    virtual ~Ping();
    /** @brief Initialization of omnetpp.ini parameters*/
    virtual int numInitStages() const override {
      return inet::NUM_INIT_STAGES;
    }
    virtual void initialize(int) override;
    virtual void handleMessage(omnetpp::cMessage*) override;
};

#endif /* APP_PONG_H_ */
