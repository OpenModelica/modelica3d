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

package Modelica3D

  import ModelicaServices.modcount.Context;
  import ModelicaServices.modcount.HeapString;
  import Id = ModelicaServices.modcount.HeapString;
  import ModelicaServices.modcount.getString;

  import ModelicaServices.modbus.Connection;
  import ModelicaServices.modbus.Message;
  import ModelicaServices.modbus.sendMessage;
  import ModelicaServices.modbus.addInteger;
  import ModelicaServices.modbus.addReal;
  import ModelicaServices.modbus.addString;

  constant String TARGET = "de.tuberlin.uebb.modelica3d.server";
  constant String OBJECT = "/de/tuberlin/uebb/modelica3d/server";
  constant String INTERFACE = "de.tuberlin.uebb.modelica3d.api";

  record State
    Connection conn;    
    Context context;    
    Integer frame;
  end State;

  class Controller
    discrete State state(conn=Connection("de.tuberlin.uebb.modelica3d.client"), context=Context());
    
    parameter Integer framerate = 30;
    parameter Modelica.SIunits.Time updateInterval = 1 / framerate;
    output Boolean send;     

    equation 
      send = sample(1e-08, updateInterval);

    algorithm   
      when send then
        state.frame := integer( time/updateInterval + 1); // First frame is 1, not 0;
      end when;
     
      when terminal() then
        stop(state);
      end when;
  end Controller;

 /*
  this is totally bizarre: omc wants to make a shared lib out of this function ...
  function unknownId
    input String str = "UNKNOWN";
    output Id id;
    algorithm
    id := HeapString(str);
  end unknownId;
*/

  function objectId
    input String s;
    output Id id = HeapString(s);
  end objectId;

  function createMaterial
    input State state;
    output Id id;
    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_material");	        
    algorithm
      id := HeapString("material_" + String(modcount.increase_get(state.context)));  	  
      addString(msg, "reference", getString(id));
      sendMessage(state.conn, msg);	
  end createMaterial;

  function applyMaterial
    input State state;
    input Id obj;
    input Id mat;
    output String res;
    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "apply_material");	        
    algorithm        	  
      addString(msg, "reference", getString(obj));
      addString(msg, "material", getString(mat));
      res := sendMessage(state.conn, msg);
  end applyMaterial;

  function createBox
    input State state;
    input Real length, width, height;
    output Id id;
    
    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_box");	        
        
    algorithm
	id := HeapString("box_" + String(modcount.increase_get(state.context)));  	  
      	addString(msg, "reference", getString(id));
	addReal(msg, "length", length);
	addReal(msg, "width", width);
	addReal(msg, "height", height);
	sendMessage(state.conn, msg);	
  end createBox;

  function createBoxAt
    input State state;
    input Real length, width, height;
    input Real tx,ty,tz;
    output Id id;
    
    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_box");	        
        
    algorithm
	id := HeapString("box_" + String(modcount.increase_get(state.context)));  	  
      	addString(msg, "reference", getString(id));
	addReal(msg, "length", length);
	addReal(msg, "width", width);
	addReal(msg, "height", height);
	addReal(msg, "tx", tx);
	addReal(msg, "ty", ty);
	addReal(msg, "tz", tz);
	sendMessage(state.conn, msg);	
  end createBoxAt;

  function createSphere
    input State state;    
    input Real size;  	  
    
    output Id id;
    protected Message msg = Message(TARGET, OBJECT, INTERFACE, "make_sphere");	        
        
    algorithm
        id := HeapString("sphere_" + String(modcount.increase_get(state.context)));
      	addString(msg, "reference", getString(id));
	addReal(msg, "size", size);
	sendMessage(state.conn, msg);
  end createSphere;

  function createCylinder
    input State state;
    input Real height;
    input Real diameter;

    output Id id;
    protected Message msg = Message(TARGET, OBJECT, INTERFACE, "make_cylinder");	        

    algorithm
        id := HeapString("cylinder_" + String(modcount.increase_get(state.context)));
      	addString(msg, "reference", getString(id));
	addReal(msg, "height", height);	
	addReal(msg, "diameter", diameter);
	sendMessage(state.conn, msg);		
  end createCylinder;

  function createCylinderAt
    input State state;
    input Real height;
    input Real diameter;
    input Real x,y,z;

    output Id id;
    protected Message msg = Message(TARGET, OBJECT, INTERFACE, "make_cylinder");	        

    algorithm
        id := HeapString("cylinder_" + String(modcount.increase_get(state.context)));
      	addString(msg, "reference", getString(id));
	addReal(msg, "height", height);	
	addReal(msg, "diameter", diameter);
        addReal(msg, "x", x);
        addReal(msg, "y", y);
        addReal(msg, "z", z);
	sendMessage(state.conn, msg);		
  end createCylinderAt;

  function createCone
    input State state;
    input Real height;
    input Real diameter;
    output Id id;
    protected Message msg = Message(TARGET, OBJECT, INTERFACE, "make_cone");	        
        
    algorithm
        id := HeapString("cone_" + String(modcount.increase_get(state.context)));
      	addString(msg, "reference", getString(id));
	addReal(msg, "height", height);	
	addReal(msg, "diameter", diameter);
	sendMessage(state.conn, msg);		
  end createCone;

  function createConeAt
    input State state;
    input Real height;
    input Real diameter;
    input Real x,y,z;
    output Id id;
    protected Message msg = Message(TARGET, OBJECT, INTERFACE, "make_cone");	        
        
    algorithm
        id := HeapString("cone_" + String(modcount.increase_get(state.context)));
      	addString(msg, "reference", getString(id));
	addReal(msg, "height", height);	
	addReal(msg, "diameter", diameter);
        addReal(msg, "x", x);
        addReal(msg, "y", y);
        addReal(msg, "z", z);
	sendMessage(state.conn, msg);		
  end createConeAt;


  function moveTo
    input State state;
    input Id id;
    input Real p[3];
    input Integer frame=state.frame;    
    output String r;
    
    protected 
    Message msg = Message(TARGET, OBJECT, INTERFACE, "move_to");
    algorithm
      	addString(msg, "reference", getString(id));
	addReal(msg, "x", p[1]);
	addReal(msg, "y", p[2]);
	addReal(msg, "z", p[3]);
	addInteger(msg, "frame", frame);
	r := sendMessage(state.conn, msg);		
  end moveTo;  

  function moveZ
    input State state;
    input Id id;
    input Real z;
    input Integer frame=state.frame;    
    output String r;
    
    protected 
    Message msg = Message(TARGET, OBJECT, INTERFACE, "move_to");
    algorithm
      	addString(msg, "reference", getString(id));
	addReal(msg, "z", z);
	addInteger(msg, "frame", frame);
	r := sendMessage(state.conn, msg);		
  end moveZ;  

  function scale
    input State state;
    input Id id;
    input Real x,y,z;
    input Integer frame=state.frame;
    
    output String r;
    
    protected 
    Message msg = Message(TARGET, OBJECT, INTERFACE, "scale");
    algorithm
      	addString(msg, "reference", getString(id));
	addReal(msg, "x", x);
	addReal(msg, "y", y);
	addReal(msg, "z", z);
	addInteger(msg, "frame", frame);
	r := sendMessage(state.conn, msg);		
  end scale;  

  function scaleZ
    input State state;
    input Id id;
    input Real z;
    input Integer frame=state.frame;
    
    output String r;
    
    protected 
    Message msg = Message(TARGET, OBJECT, INTERFACE, "scale");
    algorithm
      	addString(msg, "reference", getString(id));
	addReal(msg, "z", z);
	addInteger(msg, "frame", frame);
	r := sendMessage(state.conn, msg);		
  end scaleZ;  
  
  function setAmbientColor
    input State state;
    input Id id;    
    input Real r,g,b,a;
    input Integer frame=state.frame;
    output String res;
    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "set_ambient_color");
    algorithm
      	addString(msg, "reference", getString(id));       
        addReal(msg, "r", r);
        addReal(msg, "g", g);
        addReal(msg, "b", b);
        addReal(msg, "a", a);
	addInteger(msg, "frame", frame);
	res := sendMessage(state.conn, msg);
  end setAmbientColor;

 function setDiffuseColor
    input State state;
    input Id id;    
    input Real r,g,b,a;
    input Integer frame=state.frame;
    output String res;
    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "set_diffuse_color");
    algorithm
      	addString(msg, "reference", getString(id));        
        addReal(msg, "r", r);
        addReal(msg, "g", g);
        addReal(msg, "b", b);
        addReal(msg, "a", a);
	addInteger(msg, "frame", frame);
	res := sendMessage(state.conn, msg);
  end setDiffuseColor;

 function setSpecularColor
    input State state;
    input Id id;    
    input Real r,g,b,a;
    input Integer frame=state.frame;
    output String res;
    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "set_specular_color");
    algorithm
      	addString(msg, "reference", getString(id));        
        addReal(msg, "r", r);
        addReal(msg, "g", g);
        addReal(msg, "b", b);
        addReal(msg, "a", a);
	addInteger(msg, "frame", frame);
	res := sendMessage(state.conn, msg);
  end setSpecularColor;

  function setMatProperty
    input State state;
    input Id id;
    input String property;
    input Real value;
    input Integer frame=state.frame;
    output String res;
    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "set_material_property");
    algorithm
      	addString(msg, "reference", getString(id));
        addString(msg, "prop", property);
        addReal(msg, "value", value);
	addInteger(msg, "frame", frame);
	res := sendMessage(state.conn, msg);		
  end setMatProperty;

  function loadSceneFromFile
    input State state;
    input String fileName;
    output String res;
    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "load_scene");
    algorithm
        addString(msg, "filepath", fileName);
        res := sendMessage(state.conn, msg);
  end loadSceneFromFile;

  function rotate
    input State state;
    input Id id;
    input Real[3,3] R;
    input Integer frame=state.frame;
        
    output String r;

    protected 
    Message msg = Message(TARGET, OBJECT, INTERFACE, "rotate");        
    algorithm
      	addString(msg, "reference", getString(id));
	
	/* arrays not yet supported by dbus layer */
	for i in 1:3 loop
          for j in 1:3 loop
            addReal(msg, "R_" + String(i) + "_" + String(j), R[i,j]);
          end for;
        end for;

	addInteger(msg, "frame", frame);
	r := sendMessage(state.conn, msg);
  end rotate;

  function stop
    input State state;
    output String r;     
    protected Message msg = Message(TARGET, OBJECT, INTERFACE, "stop");
    algorithm
	r := sendMessage(state.conn, msg);
  end stop;

end Modelica3D;