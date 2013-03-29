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

#include <queue>
#include <vector>

#include "operations.hpp"

namespace proc3d {
  
  struct get_time : boost::static_visitor<double> {
    template <typename T>
    double operator()(const T& op) const {
      return op.time;
    }
  };

  static inline double time_of(const AnimOperation& op) {
    return boost::apply_visitor( get_time(), op );
  }
  
  struct compare_frames : boost::static_visitor<bool> {
    template <typename T1, typename T2>
    bool operator()(const T1& op1, const T2& op2) const {
      return op1.time > op2.time;
    }
  };

  struct AnimationComparator {
    bool operator()(const AnimOperation& op1, const AnimOperation& op2) const {
      return boost::apply_visitor( compare_frames(), op1, op2 );
    }
  };

  typedef std::priority_queue<AnimOperation, std::vector<AnimOperation>, AnimationComparator> animation_queue;
  class AnimationContext {
  public:
    std::queue<SetupOperation> setupOps;
    animation_queue deltaOps;

    virtual void handleSignal(const int signal) {
    };
  };

}
