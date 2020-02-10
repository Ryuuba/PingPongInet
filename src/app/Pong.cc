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

Define_Module(Pong);

Pong::Pong() : timer(nullptr), msg_counter(0), seq(0)
{

}

Pong::~Pong()
{
    cancelAndDelete(timer);
}

void Pong::initialize(int stage) {
  cSimpleModule::initialize(stage);
  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    inet::registerService(inet::Protocol::pong, gate("inputPort"), nullptr);
    inet::registerProtocol(inet::Protocol::pong, gate("outputPort"), nullptr);
    input_gate_id = gate("inputPort")->getId();
    output_gate_id = gate("outputPort")->getId();
    pong_delay = par("pongInterval");
    packet_size = par("packetSize");
    node_status = NO_INFO;
    timer = new omnetpp::cMessage;
    WATCH(node_status);
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
  else if(msg->arrivedOn(input_gate_id)) {
    process_ping(msg);
    scheduleAt(omnetpp::simTime() + pong_delay, timer);
    node_status = INFO;
  }

}

void Pong::send_packet( ) {
  inet::Packet* pong_pkt = new inet::Packet("reply");
  //Constructs an object of type T and wraps it in a inet::Ptr 
  const auto& pong_reply = inet::makeShared<inet::PingPongPacket>();
  pong_reply->setChunkLength(inet::B(packet_size));
  pong_reply->setType(inet::PONG);
  pong_reply->setIdentifier(555);
  pong_reply->setSeqNumber(seq++);
  pong_pkt->insertAtBack(pong_reply);
  // set sending protocol
  pong_pkt->addTagIfAbsent<inet::DispatchProtocolReq>()->setProtocol(&inet::Protocol::ping);
  // determine receiving gate
  pong_pkt->addTagIfAbsent<inet::DispatchProtocolReq>()->setServicePrimitive(inet::SP_RESPONSE);
  //This tag specifies the protocol of the packet.
  pong_pkt->addTagIfAbsent<inet::PacketProtocolTag>()->setProtocol(&inet::Protocol::ping);
  send(pong_pkt, output_gate_id);
}

void Pong::process_ping(omnetpp::cMessage* msg) {
  delete msg;
}
