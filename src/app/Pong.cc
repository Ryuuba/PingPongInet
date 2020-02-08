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

#include "Pong.h"
#include "inet/common/IProtocolRegistrationListener.h"

Pong::Pong() : timer(nullptr), msg_counter(0)
{

}

Pong::~Pong()
{
    cancelAndDelete(timer);
}

void Pong::initialize(int stage) {
  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    inet::registerService(inet::Protocol::Pong, nullptr, gate("dispatcherIn"));
    inet::registerProtocol(inet::Protocol::Pong, gate("dispatcherOut"), nullptr);
    input_gate_id = gate("inputPort")->getId();
    output_gate_id = gate("outputPort")->getId();
    pong_delay = par("pong_Interval");
    packet_size = par("packetSize");
    node_status = NO_INFO;
  }
}

void Pong::handleMessage(omnetpp::cMessage* msg) {
  if(msg->isSelfMessage())
    switch (node_status)
    {
    case NO_INFO:
      throw omnetpp::cRuntimeError("Pong: node tries to emit a message without info");
      break;
    case INFO:
      send_packet();
      node_status = NO_INFO;
      break;
    default:
      throw omnetpp::cRuntimeError("Pong: unknown timer\n");
      break;
    }
  else if(msg->arrivedOn(input_gate_id))
    process_ping(msg);
}

void Pong::send_packet( ) {
  inet::Packet* pong_pkt = new inet::Packet("reply");
  //Constructs an object of type T and wraps it in a inet::Ptr 
  const auto& pong_reply = inet::makeShared<inet::PingPongPacket>();
  pong_reply->setChunkLength(inet::B(packet_size));
  pong_reply->setType(inet::PONG);
  pong_reply->setIdentifier(555);
  pong_reply->setSeqNumber(++seq);
  pong_pkt->insertAtBack(pong_reply);
  
}

void Pong::process_ping(omnetpp::cMessage* msg) {
  req = inet::check_and_cast<inet::PingPongPacket*>(msg);
  seq = req->getSeqNumber();
  scheduleAt(omnetpp::simTime() + pong_delay, timer);
  node_status = INFO;

}
