/*
  Copyright (c) 2012 Christoph Höger, All Rights Reserved
  
  This file is part of modelica3d 
  (https://mlcontrol.uebb.tu-berlin.de/redmine/projects/modelica3d-public).

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
   
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

within ModelicaServices;

package modbus

  model Foo
    Connection c = Connection("myName");
  end Foo;

  class Connection
    extends ExternalObject;

    function constructor 
      annotation(Include = "#include <modbus.h>", Library = {"modbus", "dbus-1"});
      input String clientName;
      output Connection conn;
      external "C" conn = modbus_acquire_session_bus(clientName);
    end constructor;

    function destructor 
      annotation(Include = "#include <modbus.h>", Library = {"modbus", "dbus-1"});
      input Connection conn;
      external "C" modbus_release_bus(conn);
    end destructor;
    
  end Connection;

  class Message
    extends ExternalObject;

    function constructor 
      annotation(Include = "#include <modbus.h>", Library = {"modbus", "dbus-1"});
      input String target;
      input String object;
      input String interface;
      input String method;      

      output Message msg;
      external "C" msg = modbus_msg_alloc(target, object, interface, method);
    end constructor;

    function destructor 
      annotation(Include = "#include <modbus.h>", Library = {"modbus", "dbus-1"});
      input Message msg;
      external "C" modbus_msg_release(msg);
    end destructor;
    
  end Message;

  function sendMessage
    input Connection conn;
    input Message msg;
    output String result;
    external "C" result = modbus_connection_send_msg(conn, msg);
  end sendMessage;

  function addReal
    input Message msg;
    input String name;
    input Real val;
    external "C" modbus_msg_add_double(msg, name, val);
  end addReal;

  function addInteger
    input Message msg;
    input String name;
    input Integer val;
    external "C" modbus_msg_add_int(msg, name, val);
  end addInteger;

  function addString
    input Message msg;
    input String name;
    input String val;
    external "C" modbus_msg_add_string(msg, name, val);
  end addString;
  
end modbus;