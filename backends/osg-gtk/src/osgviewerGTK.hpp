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

#include "animationContext.hpp"
#include "operations.hpp"

/* signal definitions */
#define RUN_ANIMATION 1

extern int run_viewer(const proc3d::AnimationContext& context);

class GTKAnimationContext : public proc3d::AnimationContext {
  
  virtual void handleSignal(const int signal) {
    switch (signal) {
    case RUN_ANIMATION: run_viewer(*this);
    }
  }
};

extern "C" {
  
  void* osg_gtk_alloc_context();

  void osg_gtk_free_context(void* context);
  
}

