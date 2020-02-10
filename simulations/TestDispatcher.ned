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

package dispatchertest.simulations;
import dispatchertest.app.Ping;
import dispatchertest.app.Pong;
import inet.common.MessageDispatcher;


//
// TODO auto-generated type
//
network TestDispatcher
{
    parameters:
        @display("bgb=250,250");
    submodules:
        ping: Ping {
            @display("p=181,69");
        }
        pong: Pong {
            @display("p=66,69");
        }
        bus: MessageDispatcher {
            parameters:
                @display("p=129,197;b=200,5,,,,1");
        }
    connections:
        ping.inputPort <-- bus.out++;
        ping.outputPort --> bus.in++;
        pong.inputPort <-- bus.out++;
        pong.outputPort --> bus.in++;
}
