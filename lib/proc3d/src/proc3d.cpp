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

#include "proc3d.hpp"
#include "operations.hpp"
#include "animationContext.hpp"

#include <boost/array.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::assign;

namespace proc3d {

  static inline AnimationContext* getContext(void* ptr) {
    return (AnimationContext*) ptr;
  }

  extern "C" {

    /* memory management */

    void* proc3d_animation_context_new() {
      return new AnimationContext();
    }

    void proc3d_animation_context_free(void* context) {
      delete getContext(context);
    }

    /* setup ops */

    void proc3d_load_object(void* context, const char* name, const char* filename, const double x, const double y, const double z) {
      getContext(context)->setupOps.push(LoadObject(name, filename, boost::array<double, 3>{x,y,z}));
    }
	
    void proc3d_create_group(void* context, const char* name) {
      getContext(context)->setupOps.push(CreateGroup(name));    
    }

    void proc3d_create_material(void* context, const char* name, const double r, const double g, const double b, const double a) {
      getContext(context)->setupOps.push(CreateMaterial(name));
    }

    void proc3d_create_sphere(void* context, const char* name, const double radius) {
      getContext(context)->setupOps.push(CreateSphere(name, radius));
    }

    void proc3d_create_box(void* context, const char* name, 
			   const double x, const double y, const double z,
			   const double width, const double length, const double height) {
      getContext(context)->setupOps.push(CreateBox(name, width, length, height, boost::array<double, 3>{x,y,z}));
    }

    void proc3d_create_plane(void* context, const char* name, const double width, const double length) {
      getContext(context)->setupOps.push(CreatePlane(name, width, length));
    }

    void proc3d_create_cylinder(void* context, const char* name, const double x, const double y, const double z, const double height, const double radius) {
      getContext(context)->setupOps.push(CreateCylinder{name, radius, height, boost::array<double, 3>{x,y,z}});
    }

    void proc3d_create_cone(void* context, const char* name, const double x, const double y, const double z, const double height, const double radius) {
      getContext(context)->setupOps.push(CreateCone(name, radius, height, boost::array<double, 3>{x,y,z}));
    }

    void proc3d_add_to_group(void* context, const char* name, const char* target) {
      getContext(context)->setupOps.push(AddToGroup(name, target));
    }

    void proc3d_apply_material(void* context, const char* name, const char* target) {
      getContext(context)->setupOps.push(ApplyMaterial(name, target));
    }

    /* delta ops */

    void proc3d_set_rotation_euler(void* context, const char* name, const double x, const double y, const double z, const double time) {
      getContext(context)->deltaOps.push(RotateEuler(name, time, x, y, z));
    }

    void proc3d_set_rotation_matrix(void* context, const char* name, 
				    const double r11, const double r12, const double r13, 
				    const double r21, const double r22, const double r23, 
				    const double r31, const double r32, const double r33, 
				    const double time) {
      boost::numeric::ublas::bounded_matrix<double, 3, 3> m;
      //TODO: This can probably be rewritten with some fancy boost function, I just can't figure out which one ...
      m(0,0) = r11; m(0,1) = r12; m(0,2) = r13;
      m(1,0) = r21; m(1,1) = r22; m(1,2) = r23;
      m(2,0) = r31; m(2,1) = r32; m(2,2) = r33;

      getContext(context)->deltaOps.push(RotateMatrix(name, time, m));
    }

    void proc3d_set_translation(void* context, const char* name, const double x, const double y, const double z, const double time) {
      getContext(context)->deltaOps.push(Move(name, time,x,y,z));
    }

    void proc3d_set_scale(void* context, const char* name, const double x, const double y, const double z, const double time) {
      getContext(context)->deltaOps.push(Scale(name, time,x,y,z));
    }

    void proc3d_set_material_property(void* context, const char* name, const char* property, const double value, const double time) {
      getContext(context)->deltaOps.push(SetMaterialProperty(name, time, property, value));
    }

    void proc3d_set_ambient_color(void* context, const char* name, const double r, const double g, const double b, const double a, const double time) {
      getContext(context)->deltaOps.push(SetAmbientColor(name, time, r, g, b, a));
    }

    void proc3d_set_specular_color(void* context, const char* name, const double r, const double g, const double b, const double a, const double time) {
      getContext(context)->deltaOps.push(SetSpecularColor(name, time, r, g, b, a));
    }

    void proc3d_set_diffuse_color(void* context, const char* name, const double r, const double g, const double b, const double a, const double time) {
      getContext(context)->deltaOps.push(SetDiffuseColor(name, time, r, g, b, a));
    }

    /* signals */

    void proc3d_send_signal(void* context, const int signal) {
      getContext(context)->handleSignal(signal);
    }

  }
}
