/*
  Copyright (c) 2012 Christoph Höger, All Rights Reserved
  
  This file is part of modelica3d 
  (https://mlcontrol.uebb.tu-berlin.de/redmine/projects/modelica3d-public).

  Licensed under the Modelica License 2
*/

within Modelica.Utilities;
package Internal
  "Internal components that a user should usually not directly utilize"
partial package PartialModelicaServices
    "Interfaces of components requiring a tool specific implementation"
  package Animation "Models and functions for 3-dim. animation"

  partial model PartialShape
        "Different visual shapes with variable size; all data have to be set as modifiers"

    import SI = Modelica.SIunits;
    import Modelica.Mechanics.MultiBody.Frames;
    import Modelica.Mechanics.MultiBody.Types;

    parameter Types.ShapeType shapeType="box"
          "Type of shape (box, sphere, cylinder, pipecylinder, cone, pipe, beam, gearwheel, spring)";
    input Frames.Orientation R=Frames.nullRotation()
          "Orientation object to rotate the world frame into the object frame"
                                                                            annotation(Dialog);
    input SI.Position r[3]={0,0,0}
          "Position vector from origin of world frame to origin of object frame, resolved in world frame"
                                                                                                      annotation(Dialog);
    input SI.Position r_shape[3]={0,0,0}
          "Position vector from origin of object frame to shape origin, resolved in object frame"
                                                                                              annotation(Dialog);
    input Real lengthDirection[3](each final unit="1")={1,0,0}
          "Vector in length direction, resolved in object frame"
                                                              annotation(Dialog);
    input Real widthDirection[3](each final unit="1")={0,1,0}
          "Vector in width direction, resolved in object frame"
                                                             annotation(Dialog);
    input SI.Length length=0 "Length of visual object"  annotation(Dialog);
    input SI.Length width=0 "Width of visual object"  annotation(Dialog);
    input SI.Length height=0 "Height of visual object"  annotation(Dialog);
    input Types.ShapeExtra extra=0.0
          "Additional size data for some of the shape types"                             annotation(Dialog);
    input Real color[3]={255,0,0} "Color of shape"               annotation(Dialog);
    input Types.SpecularCoefficient specularCoefficient = 0.7
          "Reflection of ambient light (= 0: light is completely absorbed)"
                                                                        annotation(Dialog);
    // Real rxry[3, 2];
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

       import Bus=ModelicaServices.modbus;
       import ModelicaServices.modcount;		

       import Id=ModelicaServices.modcount.HeapString;       
       import M3D=ModelicaServices.Modelica3D;		
       import Modelica.Math.Vectors.isEqual;
       
       function shapeDescrTo3D
         input M3D.State state;
         input String descr;
         input Real length, width, height;
         input Real[3] at;
         output Id id;
       protected 

       algorithm
         if (descr == "box") then
           id := M3D.createBoxAt(state, width, height, length, at[1], at[2], at[3]);
         elseif (descr == "cone") then
           id := M3D.createConeAt(state, length, width, at[1], at[2], at[3] );
         elseif (descr == "sphere") then
           id := M3D.createSphere(state, length);
         elseif (descr == "cylinder") then
           id := M3D.createCylinderAt(state, length, width, at[1], at[2], at[3]);
         elseif (descr == "pipecylinder") then
           // not yet supported
           Modelica.Utilities.Streams.print("Error: Visualization of pipecylinders has not been implemented yet!");
           id := Id("UNKNOWN");
         elseif (descr == "pipe") then
           // not yet supported
           Modelica.Utilities.Streams.print("Error: Visualization of pipes has not been implemented yet!");
           id := Id("UNKNOWN");
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
           id := M3D.loadFromFile(state, Modelica.Utilities.Files.fullPathName(descr), at[1], at[2], at[3]);
         end if;
         
       end shapeDescrTo3D;      

       outer M3D.Controller m3d_control;
       
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
         id := shapeDescrTo3D(m3d_control.state, shapeType, length, width, height, lengthDirection);
         mat := M3D.createMaterial(m3d_control.state);
         M3D.setAmbientColor(m3d_control.state, mat, color[1] / 255, color[2] / 255, color[3] / 255, 1.0, time); 
         M3D.setSpecularColor(m3d_control.state, mat, specularCoefficient * (color[1] / 255), 
                              specularCoefficient * color[2] / 255, specularCoefficient * color[3] / 255, 1.0, time);
            
         M3D.applyMaterial(m3d_control.state, id, mat);
         modcount.set(initContext, 1);
       end if;
     algorithm
       when m3d_control.send and modcount.get(initContext) == 1 then

         // check for rotation
         rotated := not Modelica.Math.Matrices.isEqual(R.T, oldT);
	     if noEvent(rotated) then
	       res := M3D.rotate(m3d_control.state, id, R.T, time);
		 end if;
	     oldT := R.T;

         // check for translation
         pos := r + Frames.resolve1(R, r_shape);         
         moved := not Modelica.Math.Vectors.isEqual(pos, pre(pos));
		 if noEvent(moved) then
		   res := M3D.moveTo(m3d_control.state, id, pos, time);
		 end if;

       end when;
  end PartialShape;
  end Animation;

    annotation (Documentation(info="<html>

<p>
This package contains interfaces of a set of functions and models used in the
Modelica Standard Library that requires a <u>tool specific implementation</u>.
There is an associated package called <u>ModelicaServices</u>. A tool vendor
should provide a proper implementation of this library for the corresponding
tool. The default implementation is \"do nothing\".
In the Modelica Standard Library, the models and functions of ModelicaServices
are used.
</p>

</html>"));
end PartialModelicaServices;

protected
package FileSystem
    "Internal package with external functions as interface to the file system"
 extends Modelica.Icons.Library;

  function mkdir "Make directory (POSIX: 'mkdir')"
    extends Modelica.Icons.Function;
    input String directoryName "Make a new directory";
  external "C" ModelicaInternal_mkdir(directoryName);

  annotation(Library="ModelicaExternalC");
  end mkdir;

  function rmdir "Remove empty directory (POSIX function 'rmdir')"
    extends Modelica.Icons.Function;
    input String directoryName "Empty directory to be removed";
  external "C" ModelicaInternal_rmdir(directoryName);

  annotation(Library="ModelicaExternalC");
  end rmdir;

  function stat "Inquire file information (POSIX function 'stat')"
    extends Modelica.Icons.Function;
    input String name "Name of file, directory, pipe etc.";
    output Types.FileType fileType "Type of file";
  external "C" fileType=  ModelicaInternal_stat(name);

  annotation(Library="ModelicaExternalC");
  end stat;

  function rename "Rename existing file or directory (C function 'rename')"
    extends Modelica.Icons.Function;
    input String oldName "Current name";
    input String newName "New name";
  external "C" ModelicaInternal_rename(oldName, newName);

  annotation(Library="ModelicaExternalC");
  end rename;

  function removeFile "Remove existing file (C function 'remove')"
    extends Modelica.Icons.Function;
    input String fileName "File to be removed";
  external "C" ModelicaInternal_removeFile(fileName);

  annotation(Library="ModelicaExternalC");
  end removeFile;

  function copyFile
      "Copy existing file (C functions 'fopen', 'getc', 'putc', 'fclose')"
    extends Modelica.Icons.Function;
    input String fromName "Name of file to be copied";
    input String toName "Name of copy of file";
  external "C" ModelicaInternal_copyFile(fromName, toName);

  annotation(Library="ModelicaExternalC");
  end copyFile;

  function readDirectory
      "Read names of a directory (POSIX functions opendir, readdir, closedir)"
    extends Modelica.Icons.Function;
    input String directory
        "Name of the directory from which information is desired";
    input Integer nNames
        "Number of names that are returned (inquire with getNumberOfFiles)";
    output String names[nNames]
        "All file and directory names in any order from the desired directory";
    external "C" ModelicaInternal_readDirectory(directory,nNames,names);

  annotation(Library="ModelicaExternalC");
  end readDirectory;

function getNumberOfFiles
      "Get number of files and directories in a directory (POSIX functions opendir, readdir, closedir)"
  extends Modelica.Icons.Function;
  input String directory "Directory name";
  output Integer result
        "Number of files and directories present in 'directory'";
  external "C" result = ModelicaInternal_getNumberOfFiles(directory);

  annotation(Library="ModelicaExternalC");
end getNumberOfFiles;

  annotation (
Documentation(info="<html>
<p>
Package <b>Internal.FileSystem</b> is an internal package that contains
low level functions as interface to the file system.
These functions should not be called directly in a scripting
environment since more convenient functions are provided
in packages Files and Systems.
</p>
<p>
Note, the functions in this package are direct interfaces to
functions of POSIX and of the standard C library. Errors
occuring in these functions are treated by triggering
a Modelica assert. Therefore, the functions in this package
return only for a successful operation. Furthermore, the
representation of a string is hidden by this interface,
especially if the operating system supports Unicode characters.
</p>
</html>"));
end FileSystem;
end Internal;
