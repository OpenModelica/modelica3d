within ModelicaServices;
package Modelica3D 
  extends Modelica.Icons.Package;

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
  end State;


  class Controller
    discrete State state(conn=Connection("de.tuberlin.uebb.modelica3d.client"), context=Context());

    parameter Integer framerate = 30;
    parameter Modelica.SIunits.Time updateInterval = 1 / framerate;
    parameter Boolean autostop = true;

    output Boolean send;

  equation
    send = sample(1e-08, updateInterval);

  algorithm
    when terminal() then
      if autostop then stop(state); end if;
    end when;
  end Controller;

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


  function loadFromFile
    input State state;
    input String fileName;
    input Real tx,ty,tz;
    output Id id;

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "loadFromFile");

  algorithm
    id := HeapString("file_" + String(modcount.increase_get(state.context)));
    addString(msg, "reference", getString(id));
    addString(msg, "fileName", fileName);
    addReal(msg, "tx", tx);
    addReal(msg, "ty", ty);
    addReal(msg, "tz", tz);
    sendMessage(state.conn, msg);
  end loadFromFile;


  function createSphere
    input State state;
    input Real size;
    output Id id;

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_sphere");

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

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_cylinder");

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

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_cylinder");

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

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_cone");

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

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_cone");

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
    input Real t;
    output String r;

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "move_to");

  algorithm
    addString(msg, "reference", getString(id));
    addReal(msg, "x", p[1]);
    addReal(msg, "y", p[2]);
    addReal(msg, "z", p[3]);
    addReal(msg, "t", t);
    r := sendMessage(state.conn, msg);
  end moveTo;


  function moveZ
    input State state;
    input Id id;
    input Real z;
    input Real t;
    output String r;

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "move_to");

  algorithm
    addString(msg, "reference", getString(id));
    addReal(msg, "z", z);
    addReal(msg, "t", t);
    r := sendMessage(state.conn, msg);
  end moveZ;


  function scale
    input State state;
    input Id id;
    input Real x,y,z;
    input Real t;
    output String r;

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "scale");

  algorithm
    addString(msg, "reference", getString(id));
    addReal(msg, "x", x);
    addReal(msg, "y", y);
    addReal(msg, "z", z);
    addReal(msg, "t", t);
    r := sendMessage(state.conn, msg);
  end scale;


  function scaleZ
    input State state;
    input Id id;
    input Real z;
    input Real t;
    output String r;

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "scale");

  algorithm
    addString(msg, "reference", getString(id));
    addReal(msg, "z", z);
    addReal(msg, "t", t);
    r := sendMessage(state.conn, msg);
  end scaleZ;


  function setAmbientColor
    input State state;
    input Id id;
    input Real r,g,b,a;
    input Real t;
    output String res;

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "set_ambient_color");

  algorithm
    addString(msg, "reference", getString(id));
    addReal(msg, "r", r);
    addReal(msg, "g", g);
    addReal(msg, "b", b);
    addReal(msg, "a", a);
    addReal(msg, "t", t);
    res := sendMessage(state.conn, msg);
  end setAmbientColor;


  function setDiffuseColor
    input State state;
    input Id id;
    input Real r,g,b,a;
    input Real t;
    output String res;

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "set_diffuse_color");

  algorithm
    addString(msg, "reference", getString(id));
    addReal(msg, "r", r);
    addReal(msg, "g", g);
    addReal(msg, "b", b);
    addReal(msg, "a", a);
    addReal(msg, "t", t);
    res := sendMessage(state.conn, msg);
  end setDiffuseColor;


  function setSpecularColor
    input State state;
    input Id id;
    input Real r,g,b,a;
    input Real t;
    output String res;

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "set_specular_color");

  algorithm
    addString(msg, "reference", getString(id));
    addReal(msg, "r", r);
    addReal(msg, "g", g);
    addReal(msg, "b", b);
    addReal(msg, "a", a);
    addReal(msg, "t", t);
    res := sendMessage(state.conn, msg);
  end setSpecularColor;


  function setMatProperty
    input State state;
    input Id id;
    input String property;
    input Real value;
    input Real t;
    output String res;

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "set_material_property");

  algorithm
    addString(msg, "reference", getString(id));
    addString(msg, "prop", property);
    addReal(msg, "value", value);
    addReal(msg, "t", t);
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
    input Real t;
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

    addReal(msg, "t", t);
    r := sendMessage(state.conn, msg);
  end rotate;


  function stop
    input State state;
    output String r;

    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "stop");

    algorithm
      r := sendMessage(state.conn, msg);
    end stop;


  partial model PartialShape
  "Different visual shapes with variable size; all data have to be set as modifiers"
  extends
    Modelica.Utilities.Internal.PartialModelicaServices.Animation.PartialShape;

    import Bus = ModelicaServices.modbus;
    import ModelicaServices.modcount;
    import Id = ModelicaServices.modcount.HeapString;
    import Modelica.Mechanics.MultiBody.Frames;

    function shapeDescrTo3D
      input State state;
      input String descr;
      input Real length, width, height;
      input Real[3] at;
      input Real extra;
      output Id id;

    algorithm
      if (descr == "box") then
        id := createBoxAt(state, width, height, length, at[1], at[2], at[3]);
      elseif (descr == "cone") then
        id := createConeAt(state, length, width, at[1], at[2], at[3]);
      elseif (descr == "sphere") then
        id := createSphere(state, length);
      elseif (descr == "cylinder") then
        id := createCylinderAt(state, length, width, at[1], at[2], at[3]);
      elseif (descr == "pipecylinder") then
        if (Modelica.Math.isEqual(extra, 0.0)) then
          id := createCylinderAt(state, length, width, at[1], at[2], at[3]);
        else
          // not yet supported
          Modelica.Utilities.Streams.print("Error: Visualization of pipecylinders has not been implemented yet!");
          id := Id("UNKNOWN");
        end if;
      elseif (descr == "pipe") then
        if (Modelica.Math.isEqual(extra, 0.0)) then
          id := createCylinderAt(state, length, width, at[1], at[2], at[3]);
        else
          // not yet supported
          Modelica.Utilities.Streams.print("Error: Visualization of pipes has not been implemented yet!");
          id := Id("UNKNOWN");
        end if;
      elseif (descr == "beam") then
        // not yet supported
        Modelica.Utilities.Streams.print("Error: Visualization of beams has not been implemented yet!");
        id := Id("UNKNOWN");
      elseif (descr == "gearwheel") then
        // not yet supported
        Modelica.Utilities.Streams.print("Error: Visualization of gearwheels has not been implemented yet!");
        id := Id("UNKNOWN");
      elseif (descr == "spring") then
        // not yet supported
        Modelica.Utilities.Streams.print("Error: Visualization of springs has not been implemented yet!");
        id := Id("UNKNOWN");
      else
        // assume it is a filename
        id := loadFromFile(state, Modelica.Utilities.Files.fullPathName(ModelicaServices.ExternalReferences.loadResource(descr)), at[1], at[2], at[3]);
      end if;

    end shapeDescrTo3D;

    outer Controller m3d_control;

    Id id;
    Id mat;
    modcount.Context initContext = modcount.Context();
    String res;

    discrete Real[3] pos;
    discrete Real[3,3] oldT;
    discrete Boolean moved;
    discrete Boolean rotated;
  // disable initial equation for now, since there is some bug in initial residuals
  //initial equation
  //  pre(oldT) = R.T;
  //  pre(pos) = r + Frames.resolve1(R, r_shape);
  initial algorithm
    if modcount.get(initContext) <> 1 then
      id := shapeDescrTo3D(m3d_control.state, shapeType, length, width, height, lengthDirection, extra);
      mat := createMaterial(m3d_control.state);
      setAmbientColor(m3d_control.state, mat, color[1] / 255, color[2] / 255, color[3] / 255, 1.0, time);
      setSpecularColor(m3d_control.state, mat, specularCoefficient * (color[1] / 255),
                       specularCoefficient * color[2] / 255, specularCoefficient * color[3] / 255, 1.0, time);

      applyMaterial(m3d_control.state, id, mat);
      modcount.set(initContext, 1);
    end if;
  algorithm	
    when m3d_control.send and modcount.get(initContext) == 1 then
	  // check for rotation
      rotated := not Modelica.Math.Matrices.isEqual(R.T, oldT);
      if noEvent(rotated) then
        res := rotate(m3d_control.state, id, R.T, time);
      end if;
      oldT := R.T;

      // check for translation
      pos := r + Frames.resolve1(R, r_shape);
      moved := not Modelica.Math.Vectors.isEqual(pos, pre(pos));
      if noEvent(moved) then
        res := moveTo(m3d_control.state, id, pos, time);
      end if;

    end when;

    annotation (
      Icon(coordinateSystem(
          preserveAspectRatio=true,
          extent={{-100,-100},{100,100}},
          grid={2,2}), graphics={
              Rectangle(
                extent={{-100,-100},{80,60}},
                lineColor={0,0,255},
                fillColor={255,255,255},
                fillPattern=FillPattern.Solid),
              Polygon(
                points={{-100,60},{-80,100},{100,100},{80,60},{-100,60}},
                lineColor={0,0,255},
                fillColor={192,192,192},
                fillPattern=FillPattern.Solid),
              Polygon(
                points={{100,100},{100,-60},{80,-100},{80,60},{100,100}},
                lineColor={0,0,255},
                fillColor={160,160,164},
                fillPattern=FillPattern.Solid),
              Text(
                extent={{-100,-100},{80,60}},
                lineColor={0,0,0},
                textString="%shapeType"),
              Text(
                extent={{-132,160},{128,100}},
                textString="%name",
                lineColor={0,0,255})}),
      Documentation(info="<html>

<p>
This model is documented at
<a href=\"Modelica://Modelica.Mechanics.MultiBody.Visualizers.Advanced.Shape\">Modelica.Mechanics.MultiBody.Visualizers.Advanced.Shape</a>.
</p>

</html>
"));
  end PartialShape;
end Modelica3D;
