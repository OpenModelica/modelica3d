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

