within ModelicaServices;
package Modelica3D
  extends Modelica.Icons.Package;

  import Id = ModelicaServices.modcount.HeapString;
  import ModelicaServices.modcount.{Context,HeapString,getString,setString};

  import ModelicaServices.modbus.{Connection,Message,sendMessage,addInteger,addReal,addString};

  constant String TARGET = "de.tuberlin.uebb.modelica3d.server";
  constant String OBJECT = "/de/tuberlin/uebb/modelica3d/server";
  constant String INTERFACE = "de.tuberlin.uebb.modelica3d.api";

  class Controller
    discrete Connection conn = Connection("de.tuberlin.uebb.modelica3d.client");
    discrete Context context = Context();

    parameter Integer framerate = 30;
    parameter Modelica.SIunits.Time updateInterval = 1 / framerate;
    parameter Boolean autostop = true;

    output Boolean send;

  equation
    send = sample(1e-08, updateInterval);

  algorithm
    when terminal() then
      if autostop then stop(conn, context); end if;
    end when;
  annotation (
    defaultComponentName="m3d_control",
    defaultComponentPrefixes="inner",
    missingInnerMessage="No \"m3d_control\" component is defined.");
  end Controller;

  function objectId
    input String s;
    output Id id = HeapString(s);
  end objectId;


  function createMaterial
    input Connection conn;
    input Context context;
    input Id id;
  protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_material");
  algorithm
    setString(id,"material_" + String(modcount.increase_get(context)));
    addString(msg, "reference", getString(id));
    sendMessage(conn, msg);
  end createMaterial;


  function applyMaterial
    input Connection conn;
    input Context context;
    input Id obj;
    input Id mat;
    output String res;
    protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "apply_material");
  algorithm
    addString(msg, "reference", getString(obj));
    addString(msg, "material", getString(mat));
    res := sendMessage(conn, msg);
  end applyMaterial;


  function createBox
    input Connection conn;
    input Context context;
    input Real length, width, height;
    input Id id;
  protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_box");
  algorithm
    setString(id,HeapString("box_" + String(modcount.increase_get(context))));
    addString(msg, "reference", getString(id));
    addReal(msg, "length", length);
    addReal(msg, "width", width);
    addReal(msg, "height", height);
    sendMessage(conn, msg);
  end createBox;


  function createBoxAt
    input Connection conn;
    input Context context;
    input Real length, width, height;
    input Real tx,ty,tz;
    input Id id;
  protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_box");
  algorithm
    setString(id,"box_" + String(modcount.increase_get(context)));
    addString(msg, "reference", getString(id));
    addReal(msg, "length", length);
    addReal(msg, "width", width);
    addReal(msg, "height", height);
    addReal(msg, "tx", tx);
    addReal(msg, "ty", ty);
    addReal(msg, "tz", tz);
    sendMessage(conn, msg);
  end createBoxAt;

  function loadFromFile
    input Connection conn;
    input Context context;
    input String fileName;
    input Real tx,ty,tz;
    input Id id;
  protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "loadFromFile");
  algorithm
    setString(id,"file_" + String(modcount.increase_get(context)));
    addString(msg, "reference", getString(id));
    addString(msg, "fileName", fileName);
    addReal(msg, "tx", tx);
    addReal(msg, "ty", ty);
    addReal(msg, "tz", tz);
    sendMessage(conn, msg);
  end loadFromFile;


  function createSphere
    input Connection conn;
    input Context context;
    input Real size;
    input Id id;
  protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_sphere");
  algorithm
    setString(id,"sphere_" + String(modcount.increase_get(context)));
    addString(msg, "reference", getString(id));
    addReal(msg, "size", size);
    sendMessage(conn, msg);
  end createSphere;


  function createCylinder
    input Connection conn;
    input Context context;
    input Real height;
    input Real diameter;
    input Id id;
  protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_cylinder");
  algorithm
    setString(id,"cylinder_" + String(modcount.increase_get(context)));
    addString(msg, "reference", getString(id));
    addReal(msg, "height", height);
    addReal(msg, "diameter", diameter);
    sendMessage(conn, msg);
  end createCylinder;


  function createCylinderAt
    input Connection conn;
    input Context context;
    input Real height;
    input Real diameter;
    input Real x,y,z;
    input Id id;
  protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_cylinder");
  algorithm
    setString(id,"cylinder_" + String(modcount.increase_get(context)));
    addString(msg, "reference", getString(id));
    addReal(msg, "height", height);
    addReal(msg, "diameter", diameter);
    addReal(msg, "x", x);
    addReal(msg, "y", y);
    addReal(msg, "z", z);
    sendMessage(conn, msg);
  end createCylinderAt;


  function createCone
    input Connection conn;
    input Context context;
    input Real height;
    input Real diameter;
    input Id id;
  protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_cone");
  algorithm
    setString(id,"cone_" + String(modcount.increase_get(context)));
    addString(msg, "reference", getString(id));
    addReal(msg, "height", height);
    addReal(msg, "diameter", diameter);
    sendMessage(conn, msg);
  end createCone;


  function createConeAt
    input Connection conn;
    input Context context;
    input Real height;
    input Real diameter;
    input Real x,y,z;
    input Id id;
  protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "make_cone");
  algorithm
    setString(id,"cone_" + String(modcount.increase_get(context)));
    addString(msg, "reference", getString(id));
    addReal(msg, "height", height);
    addReal(msg, "diameter", diameter);
    addReal(msg, "x", x);
    addReal(msg, "y", y);
    addReal(msg, "z", z);
    sendMessage(conn, msg);
  end createConeAt;


  function moveTo
    input Connection conn;
    input Context context;
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
    r := sendMessage(conn, msg);
  end moveTo;


  function moveZ
    input Connection conn;
    input Context context;
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
    r := sendMessage(conn, msg);
  end moveZ;


  function scale
    input Connection conn;
    input Context context;
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
    r := sendMessage(conn, msg);
  end scale;


  function scaleZ
    input Connection conn;
    input Context context;
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
    r := sendMessage(conn, msg);
  end scaleZ;


  function setAmbientColor
    input Connection conn;
    input Context context;
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
    res := sendMessage(conn, msg);
  end setAmbientColor;


  function setDiffuseColor
    input Connection conn;
    input Context context;
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
    res := sendMessage(conn, msg);
  end setDiffuseColor;


  function setSpecularColor
    input Connection conn;
    input Context context;
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
    res := sendMessage(conn, msg);
  end setSpecularColor;


  function setMatProperty
    input Connection conn;
    input Context context;
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
    res := sendMessage(conn, msg);
  end setMatProperty;


  function loadSceneFromFile
    input Connection conn;
    input Context context;
    input String fileName;
    output String res;
  protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "load_scene");
  algorithm
    addString(msg, "filepath", fileName);
    res := sendMessage(conn, msg);
  end loadSceneFromFile;


  function rotate
    input Connection conn;
    input Context context;
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
    r := sendMessage(conn, msg);
  end rotate;


  function stop
    input Connection conn;
    input Context context;
    output String r;
  protected
    Message msg = Message(TARGET, OBJECT, INTERFACE, "stop");
  algorithm
    r := sendMessage(conn, msg);
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
      input Connection conn;
      input Context context;
      input String descr;
      input Real length, width, height;
      input Real[3] at;
      input Real extra;
      input Id id;
    algorithm

      if (descr == "box") then
        createBoxAt(conn, context, width, height, length, at[1], at[2], at[3], id);
      elseif (descr == "cone") then
        createConeAt(conn, context, length, width, at[1], at[2], at[3], id);
      elseif (descr == "sphere") then
        createSphere(conn, context, length, id);
      elseif (descr == "cylinder") then
        createCylinderAt(conn, context, length, width, at[1], at[2], at[3], id);
      elseif (descr == "pipecylinder") then
        if (Modelica.Math.isEqual(extra, 0.0)) then
          createCylinderAt(conn, context, length, width, at[1], at[2], at[3], id);
        else
          // not yet supported
          Modelica.Utilities.Streams.print("Error: Visualization of pipecylinders has not been implemented yet!");
          setString(id,"UNKNOWN");
        end if;
      elseif (descr == "pipe") then
        if (Modelica.Math.isEqual(extra, 0.0)) then
          createCylinderAt(conn, context, length, width, at[1], at[2], at[3], id);
        else
          // not yet supported
          Modelica.Utilities.Streams.print("Error: Visualization of pipes has not been implemented yet!");
          setString(id,"UNKNOWN");
        end if;
      elseif (descr == "beam") then
        // not yet supported
        Modelica.Utilities.Streams.print("Error: Visualization of beams has not been implemented yet!");
        setString(id,"UNKNOWN");
      elseif (descr == "gearwheel") then
        // not yet supported
        Modelica.Utilities.Streams.print("Error: Visualization of gearwheels has not been implemented yet!");
        setString(id,"UNKNOWN");
      elseif (descr == "spring") then
        // not yet supported
        Modelica.Utilities.Streams.print("Error: Visualization of springs has not been implemented yet!");
        setString(id,"UNKNOWN");
      else
        // assume it is a filename
        loadFromFile(conn, context, Modelica.Utilities.Files.fullPathName(ModelicaServices.ExternalReferences.loadResource(descr)), at[1], at[2], at[3], id);
      end if;

    end shapeDescrTo3D;

    outer Controller m3d_control;

    Id id = Id("");
    Id mat = Id("");
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
  algorithm
    when m3d_control.send and modcount.get(initContext) == 1 then
    // check for rotation
      rotated := not Modelica.Math.Matrices.isEqual(R.T, oldT);
      if noEvent(rotated) then
        res := rotate(m3d_control.conn, m3d_control.context, id, R.T, time);
      end if;
      oldT := R.T;

      // check for translation
      pos := r + Frames.resolve1(R, r_shape);
      moved := not Modelica.Math.Vectors.isEqual(pos, pre(pos));
      if noEvent(moved) then
        res := moveTo(m3d_control.conn, m3d_control.context, id, pos, time);
      end if;

    end when;

    if modcount.get(initContext) <> 1 then
      
      Modelica.Utilities.Streams.print( "SHAPE " + getString(id) + " " + shapeType);
      Modelica.Utilities.Streams.print( "length = " + String(length));
      Modelica.Utilities.Streams.print( "width = " + String(width));
      Modelica.Utilities.Streams.print( "height = " + String(height));

      
      shapeDescrTo3D(m3d_control.conn, m3d_control.context, shapeType, length, width, height, lengthDirection, extra, id);
      createMaterial(m3d_control.conn, m3d_control.context, mat);
      setAmbientColor(m3d_control.conn, m3d_control.context, mat, color[1] / 255, color[2] / 255, color[3] / 255, 1.0, time);
      setSpecularColor(m3d_control.conn, m3d_control.context, mat, specularCoefficient * (color[1] / 255),
                       specularCoefficient * color[2] / 255, specularCoefficient * color[3] / 255, 1.0, time);

      applyMaterial(m3d_control.conn, m3d_control.context, id, mat);
      modcount.set(initContext, 1);
    end if;

    
   

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
