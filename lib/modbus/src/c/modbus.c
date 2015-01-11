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

#define DBUS_STATIC_BUILD   /* In order to link against dbus static lib. */
#include <stdio.h>
#include <string.h>
#include <dbus/dbus.h>

#include "modbus.h"

#ifdef __cplusplus
extern "C"
{
#endif

DBusError err;

typedef struct modbus_message {
  DBusMessage* msg;
  DBusMessageIter args;
  DBusMessageIter dict;
} ModbusMessage;

void* modbus_acquire_session_bus(const char * client_name) {
  dbus_error_init(&err);

  /* taken from http://www.matthew.ath.cx/misc/dbus */
  DBusConnection* conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
   
   if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Connection Error (%s)\n", err.message); 
      dbus_error_free(&err); 
   }

   if (NULL == conn) { 
      exit(1); 
   }

   // request a name on the bus
   int ret = dbus_bus_request_name(conn, client_name, 
         DBUS_NAME_FLAG_REPLACE_EXISTING 
         , &err);

   if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Name Error (%s)\n", err.message); 
      dbus_error_free(&err); 
   }

   if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) { 
      exit(1);
   }
   
   return (void*)conn;
}

void modbus_release_bus(void* vconn) {
  DBusConnection* conn = (DBusConnection*)vconn;
  dbus_connection_unref(conn);
}

void* modbus_msg_alloc(const char *target, const char* object, const char *interface, const char* method) {
  ModbusMessage* message = (ModbusMessage*)malloc(sizeof(ModbusMessage));

  message->msg = dbus_message_new_method_call(target, object, interface, method);
  if (NULL == message->msg) { 
    fprintf(stderr, "Message Null\n");
    exit(1);
  }

  dbus_message_iter_init_append(message->msg, &(message->args));
  dbus_message_iter_open_container(&(message->args), DBUS_TYPE_ARRAY,
				   DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING
				   DBUS_TYPE_STRING_AS_STRING
				   DBUS_TYPE_VARIANT_AS_STRING
				   DBUS_DICT_ENTRY_END_CHAR_AS_STRING,
				   &(message->dict));

  return message;
}

void modbus_msg_release(void* vmessage) {
  ModbusMessage* message = (ModbusMessage*)vmessage;
  // free message
  dbus_message_unref(message->msg);  
  free(message);
}

const char* modbus_connection_send_msg(void* vconn, void* vmessage) {
  DBusConnection* conn = (DBusConnection*)vconn;
  ModbusMessage* message = (ModbusMessage*)vmessage;
  DBusMessage* reply;
  DBusMessageIter rargs;
  DBusPendingCall* pending;
  char* stat;
  
  dbus_message_iter_close_container(&(message->args), &(message->dict));
  
  // send message and get a handle for a reply
  if (!dbus_connection_send_with_reply (conn, message->msg, &pending, -1)) { // -1 is default timeout
    fprintf(stderr, "Out Of Memory!\n"); 
    exit(1);
  }
  if (NULL == pending) { 
    fprintf(stderr, "Pending Call Null\n"); 
    exit(1); 
  }
  dbus_connection_flush(conn);
  
  // block until we receive a reply
  dbus_pending_call_block(pending);

  // get the reply message
  reply = dbus_pending_call_steal_reply(pending);
  if (NULL == reply) {
    fprintf(stderr, "Reply Null\n"); 
    exit(1); 
  }
  
  // free the pending message handle
  dbus_pending_call_unref(pending);
  
  // read the parameters
  if (!dbus_message_iter_init(reply, &rargs))
    fprintf(stderr, "Message has no arguments!\n"); 
  else if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&rargs)) 
    dbus_message_iter_get_basic(&rargs, &stat);

  
  stat = strdup(stat);
  // free reply and close connection
  dbus_message_unref(reply);
  
  // printf("Returning: %s\n", stat);
  return stat;
}

void msg_add_entry(const void* vmessage, const char* name, const void* val, 
		   const int type, const char* sig) {

  ModbusMessage* message = (ModbusMessage*)vmessage;

  /* prepare dictionary entry */
  DBusMessageIter entry;
  dbus_message_iter_open_container(&(message->dict), DBUS_TYPE_DICT_ENTRY, 0, &entry);
  dbus_message_iter_append_basic(&entry, DBUS_TYPE_STRING, &name);

  /* stuff value into variant */
  DBusMessageIter var;  
  dbus_message_iter_open_container(&entry, DBUS_TYPE_VARIANT, sig, &var);
  dbus_message_iter_append_basic(&var, type, val);  
  dbus_message_iter_close_container(&entry, &var);
  
  /* close dictionary entry */
  dbus_message_iter_close_container(&(message->dict), &entry);
  
}

void modbus_msg_add_double(void* message, const char* name, double value) {
  return msg_add_entry(message, name, &value, DBUS_TYPE_DOUBLE, DBUS_TYPE_DOUBLE_AS_STRING);
}

void modbus_msg_add_int(void* message, const char* name, int value) {
  return msg_add_entry(message, name, &value, DBUS_TYPE_INT32, DBUS_TYPE_INT32_AS_STRING);
}

void modbus_msg_add_string(void* message, const char* name, const char* value) {
  return msg_add_entry(message, name, &value, DBUS_TYPE_STRING, DBUS_TYPE_STRING_AS_STRING);
}

#ifdef __cplusplus
}
#endif
