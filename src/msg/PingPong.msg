//
// Copyright (C) 2004, 2009 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//
// Compilation: opp_msgc -V -I../../../inet4/src/ --msg6 PingPong.msg

import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;

namespace inet;
//
// Generic network protocol packet for testing network connectivity.
//
enum PingPongMessageType
{
  PING = 0;
  PONG = 1;
}

class PingPongPacket extends FieldsChunk
{
  PingPongMessageType type; //The type 
  int identifier; // identifier to aid in matching Echo replies. May be Zero
  int seqNumber; // sequence number to aid in matching Echo replies. May be Zero
}

