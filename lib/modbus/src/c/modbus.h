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

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

void* moddbus_acquire_session_bus();

void moddbus_release_bus(void* conn);

void* moddbus_msg_alloc(char *interface, char* object);

void moddbus_msg_release(void* msg);

void moddbus_msg_add_double(void* msg, char* name, double value);

void moddbus_msg_add_int(void* msg, char* name, uint32_t value);

void moddbus_msg_add_string(void* msg, char* name, char* value);

const char* modbus_connection_send_msg(void* vconn, void* vmessage);

#ifdef __cplusplus
}
#endif
