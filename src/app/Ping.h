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

#ifndef APP_PING_H_
#define APP_PING_H_

#include <omnetpp/csimplemodule.h>
#include "inet/common/INETDefs.h"
#include "inet/common/packet/Packet.h"

class Ping: public omnetpp::cSimpleModule {
protected:
  omnetpp::cMessage* timer;
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

#endif /* APP_PING_H_ */
