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

model Test
  import ModelicaServices.modcount;

  /* whenever this function is evaluated, increase the counter */ 
  function f  
    input modcount.Context context;
    input Real x;
    output Real y;
    
    algorithm
      modcount.increase_get(context);
      y := x;
  end f;

  Real x;
  Integer i;
  modcount.Context context = modcount.Context();

  equation
  when sample(1,1) then
    /* read the counter */
    i = modcount.get(context);
  end when;

  der(x) = f(context, time);  

end Test;