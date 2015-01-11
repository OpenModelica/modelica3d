/*
  This file is part of the Modelica3D package.
  
  Copyright (C) 2012-current year  Christoph Höger and Technical University of Berlin

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

  Main Author 2010-2013, Christoph Höger
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

void* modbus_acquire_session_bus(const char * client_name);

void modbus_release_bus(void* conn);

void* modbus_msg_alloc(const char *target, const char* object, const char *interface, const char* method);

void modbus_msg_release(void* msg);

void modbus_msg_add_double(void* msg, const char* name, double value);

void modbus_msg_add_int(void* msg, const char* name, int value);

void modbus_msg_add_string(void* msg, const char* name, const char* value);

const char* modbus_connection_send_msg(void* vconn, void* vmessage);

#ifdef __cplusplus
}
#endif
