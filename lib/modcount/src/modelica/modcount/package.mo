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

within ModelicaServices;

package modcount

  class Context
    extends ExternalObject;
    
    function constructor    
      annotation(Include = "#include <modcount.h>", Library = {"modcount"});
      output Context context;
      external "C" context = modcount_acquire_context();
    end constructor;

    function destructor
      annotation(Include = "#include <modcount.h>", Library = {"modcount"});
      input Context context;
      external "C" modcount_release_context(context);
    end destructor;
  end Context;  

  function set
    annotation(Include = "#include <modcount.h>", Library = {"modcount"});
    input Context c;
    input Integer i;
    output Integer out;
    external "C" out = modcount_set(c, i);
  end set;
  
  function get
    annotation(Include = "#include <modcount.h>", Library = {"modcount"});
    input Context c;
    output Integer i;
    external "C" i = modcount_get(c);
  end get;

  function increase_get    
    input Context c;
    output Integer i;
    algorithm
    i := get(c);
    i := set(c, i + 1);
  end increase_get;

  class HeapString
    extends ExternalObject;

    function constructor    
      annotation(Include = "#include <modcount.h>", Library = {"modcount"});
      input String content;
      output HeapString str;
      external "C" str = modcount_acquire_string(content);
    end constructor;

    function destructor
      annotation(Include = "#include <modcount.h>", Library = {"modcount"});
      input HeapString str;
      external "C" modcount_release_string(str);
    end destructor;
  end HeapString;

  function getString
    input HeapString str;
    output String val;
    annotation(Include = "#include <modcount.h>", Library = {"modcount"});
    external "C" val = modcount_get_string(str);
  end getString;
  

end modcount;