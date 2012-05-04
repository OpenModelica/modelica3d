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

#pragma once

#include <boost/variant.hpp>
#include <boost/array.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace proc3d {

  using namespace boost; //array, variant
  using namespace boost::numeric::ublas; //bounded_matrix

  struct ObjectOperation {
    ObjectOperation(const std::string& name) : name(name) {}
    std::string name;
  };

  struct CreateGroup : ObjectOperation {    
    CreateGroup(const std::string& name) : ObjectOperation(name) {}
  };

  struct LoadObject : ObjectOperation {
    LoadObject(const std::string& name) : ObjectOperation(name) {}
  };

  struct ObjectLinkOperation : ObjectOperation {
    ObjectLinkOperation(const std::string& name, const std::string& target) : ObjectOperation(name), target(target) {}
    std::string target;
  };

  struct AddToGroup : ObjectLinkOperation {
    AddToGroup(const std::string& name, const std::string& target) : ObjectLinkOperation(name, target) {}
  };

  struct CreateMaterial : ObjectOperation {     
    CreateMaterial(const std::string& name) : ObjectOperation(name) {}
  };

  struct ApplyMaterial : ObjectLinkOperation {
    ApplyMaterial(const std::string& name, const std::string& target) : ObjectLinkOperation(name, target) {}
  };

  struct CreateSphere : ObjectOperation {
    CreateSphere(const std::string& name, const double r) : ObjectOperation(name), radius(r) {}
    double radius;
  };

  struct CreateBox : ObjectOperation {
    CreateBox(const std::string& name, const double w, const double l, const double h, const array<double,3>& a) : ObjectOperation(name), width(w), length(l), height(h), at(a) {}
    double width, length, height;
    array<double,3> at;
  };

  struct CreateCylinder : ObjectOperation {
    CreateCylinder(const std::string& name, const double r, const double h, const array<double,3>& a) : ObjectOperation(name), radius(r), height(h), at(a) {}
    double radius;
    double height;
    array<double, 3> at;
  };
  
  struct CreateCone : ObjectOperation {
    CreateCone(const std::string& name, const double r, const double h, const array<double, 3>& a) : ObjectOperation(name), radius(r), height(h), at(a) {}
    double radius;
    double height;
    array<double, 3> at;
  };

  struct CreatePlane : ObjectOperation {
    CreatePlane(const std::string& name, const double l, const double w) : ObjectOperation(name), length(l), width(w) {}
    double length;
    double width;
  };

  struct DeltaOperation : ObjectOperation {
    DeltaOperation(const std::string& name, const unsigned int f) : ObjectOperation(name), frame(f) {}
    unsigned int frame;
  };
    
  struct Move : DeltaOperation {
    Move(const std::string& name, const unsigned int f, const double x, const double y, const double z) : DeltaOperation(name, f), x(x), y(y), z(z) {}
    double x,y,z;
  };

  struct Scale : DeltaOperation {
    Scale(const std::string& name, const unsigned int f, const double x, const double y, const double z) : DeltaOperation(name, f), x(x), y(y), z(z) {}
    double x,y,z;
  };

  struct RotateEuler : DeltaOperation {
    RotateEuler(const std::string& name, const unsigned int f, const double x, const double y, const double z) : DeltaOperation(name, f), x(x), y(y), z(z) {}
    double x,y,z;
  };

  struct RotateMatrix : DeltaOperation {
    RotateMatrix(const std::string& name, const unsigned int f, const bounded_matrix<double, 3, 3>& m) : DeltaOperation(name, f), m(m) {}
    bounded_matrix<double, 3, 3> m;
  };

  struct SetMaterialProperty : DeltaOperation {
    SetMaterialProperty(const std::string& name, const unsigned int f, const std::string& p, const double v) : DeltaOperation(name, f), property(p), value(v) {}
    std::string property;
    double value;
  };

  struct SetAmbientColor : DeltaOperation {
    SetAmbientColor(const std::string& name, const unsigned int f, 
		    const double r, const double g, const double b, const double a) : 
      DeltaOperation(name, f), color({{r,g,b,a}}) {}
    array<double, 4> color;
  };

  struct SetDiffuseColor : DeltaOperation {
    SetDiffuseColor(const std::string& name, const unsigned int f, 
		    const double r, const double g, const double b, const double a) : 
      DeltaOperation(name, f), color({{r,g,b,a}}) {}
    array<double, 4> color;
  };

  struct SetSpecularColor : DeltaOperation {
    SetSpecularColor(const std::string& name, const unsigned int f, 
		     const double r, const double g, const double b, const double a) : 
      DeltaOperation(name, f), color({{r,g,b,a}}) {}
    array<double, 4> color;
  };

  typedef variant<CreateGroup, CreateSphere, CreateBox, 
		  CreateCylinder, CreateCone, CreatePlane,
		  LoadObject, AddToGroup, CreateMaterial, ApplyMaterial> SetupOperation;

  typedef variant<Move, Scale, RotateEuler, RotateMatrix, SetMaterialProperty, 
		  SetAmbientColor, SetDiffuseColor, SetSpecularColor> AnimOperation;
  
}


