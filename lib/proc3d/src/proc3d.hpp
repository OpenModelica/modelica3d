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

#pragma once

#include <queue>

#include "operations.hpp"

extern "C" {
  
  /* memory management */

  void* proc3d_animation_context_new();

  void proc3d_animation_context_free(void* pAnimationContext);

  /* setup ops */

  void proc3d_load_object(void* context, const char* name, const char* filename, const double x, const double y, const double z);
	
  void proc3d_create_group(void* context, const char* name);

  void proc3d_create_material(void* context, const char* name, const double r, const double g, const double b, const double a);

  void proc3d_create_sphere(void* context, const char* name, const double radius);

  void proc3d_create_box(void* context, const char* name, 
			 const double tx, const double ty, const double tz,
			 const double width, const double length, const double height);

  void proc3d_create_plane(void* context, const char* name, const double width, const double length);

  void proc3d_create_cylinder(void* context, const char* name, 
			      const double tx, const double ty, const double tz, 
			      const double height, const double radius);

  void proc3d_create_cone(void* context, const char* name, 
			  const double tx, const double ty, const double tz, 
			  const double height, const double radius);

  void proc3d_add_to_group(void* context, const char* name, const char* target);

  void proc3d_apply_material(void* context, const char* name, const char* target);

  /* delta ops */

  void proc3d_set_rotation_euler(void* context, const char* name, const double x, const double y, const double z, const double time);

  void proc3d_set_rotation_matrix(void* context, const char* name, 
				 const double r11, const double r12, const double r13, 
				 const double r21, const double r22, const double r23, 
				 const double r31, const double r32, const double r33, 
				 const double time);

  void proc3d_set_translation(void* context, const char* name, const double x, const double y, const double z, const double time);

  void proc3d_set_scale(void* context, const char* name, const double x, const double y, const double z, const double time);

  void proc3d_set_material_property(void* context, const char* name, const char* property, const double value, const double time);

  /* coloring */
  void proc3d_set_ambient_color(void* context, const char* name, const double r, const double g, const double b, const double a, const double time);

  void proc3d_set_specular_color(void* context, const char* name, const double r, const double g, const double b, const double a, const double time);

  void proc3d_set_diffuse_color(void* context, const char* name, const double r, const double g, const double b, const double a, const double time);

  /* signals */

  void proc3d_send_signal(void* context, const int signal);

}






