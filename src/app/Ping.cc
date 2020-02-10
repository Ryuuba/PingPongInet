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

#include "Ping.h"
#include "inet/common/IProtocolRegistrationListener.h"

Define_Module(Ping);

Ping::Ping() : timer(nullptr), msg_counter(0), seq(0)
{

}

Ping::~Ping()
{
    cancelAndDelete(timer);
}

void Ping::initialize(int stage) {
  cSimpleModule::initialize(stage);
  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    inet::registerService(inet::Protocol::ping, gate("inputPort"), nullptr);
    inet::registerProtocol(inet::Protocol::ping, gate("outputPort"), nullptr);
    input_gate_id = gate("inputPort")->getId();
    output_gate_id = gate("outputPort")->getId();
    ping_delay = par("pingInterval");
    packet_size = par("packetSize");
    node_status = INFO;
    timer = new omnetpp::cMessage;
    scheduleAt(omnetpp::simTime() + ping_delay, timer);
    WATCH(node_status);
  }
}

void Ping::handleMessage(omnetpp::cMessage* msg) {
  if(msg->isSelfMessage()) {
    switch (node_status)
    {
      case INFO:
        send_packet();
        node_status = NO_INFO;
        break;
      default:
        throw omnetpp::cRuntimeError("Ping: unknown timer\n");
        break;
    }
  }
  else if(msg->arrivedOn(input_gate_id)) {
    process_pong(msg);
    scheduleAt(omnetpp::simTime() + ping_delay, timer);
    node_status = INFO;
  }
}

void Ping::send_packet( ) {
  inet::Packet* ping_pkt = new inet::Packet("request");
  //Constructs an object of type T and wraps it in a inet::Ptr 
  const auto& ping_req = inet::makeShared<inet::PingPongPacket>();
  ping_req->setChunkLength(inet::B(packet_size));
  ping_req->setType(inet::PING);
  ping_req->setIdentifier(555);
  ping_req->setSeqNumber(seq++);
  ping_pkt->insertAtBack(ping_req);
  // set destination protocol
  ping_pkt->addTagIfAbsent<inet::DispatchProtocolReq>()->setProtocol(&inet::Protocol::pong);
  // determine receiving gate
  ping_pkt->addTagIfAbsent<inet::DispatchProtocolReq>()->setServicePrimitive(inet::SP_REQUEST);
  //This tag specifies the protocol of the packet.
  ping_pkt->addTagIfAbsent<inet::PacketProtocolTag>()->setProtocol(&inet::Protocol::ping);
  send(ping_pkt, output_gate_id);
}

void Ping::process_pong(omnetpp::cMessage* msg) {
  delete msg;
}
