from dbus.mainloop.glib import DBusGMainLoop
import dbus
import dbus.service

DBusGMainLoop(set_as_default=True)

from gi.repository import GObject

from bpy import ops, data, context
from mathutils import Matrix

from itertools import tee, product

l = GObject.MainLoop()

# Wrapper for dbus api decorator
dec = dbus.service.method(dbus_interface='de.tuberlin.uebb.modelica3d.api',
                          in_signature='a{sv}',
                          out_signature='s')

# decorate a function with optional typechecks
def mod3D_api(**checks):
    def tc(f):
        print("Decorating %s with %s" % (str(f), checks))

        def checked(s, **p) :    
            print("Got: %s %s" % (str(s), str(p)))
            for param in p:
                if param in checks:
                    res,msg = checks[param](p[param])
                    if not res:
                        return msg
            return f(s, **p)

        nf = lambda s, p={} : checked(s, **p)
        nf.__name__ = f.__name__
        return dec(nf)

    return tc       

# Check for undefinded/defined references
def undefined_object(ref) :
    return (not ref in data.objects, "%s is already in use." % str(ref))

def defined_object(ref) :
    return (ref in data.objects, "%s undefined" % str(ref))

def undefined_material(ref) :
    return (not ref in data.materials, "%s is already in use." % str(ref))

def defined_material(ref) :
    return (ref in data.materials, "%s undefined" % str(ref))

# Check for positive value
def positive(s):
    return (s <= 0.0, "expected positive value")

def positive_int(i):
    return (isinstance(i, int) and i >= 0, "expected positive int")

def not_zero(x):
    return (i != 0, "parameter may not be zero")

class Modelica3DAPI(dbus.service.Object):

    @mod3D_api()
    def stop(self):
        l.quit()
        return "stopped"

    @mod3D_api(reference = undefined_object, length = not_zero)
    def make_box(self, reference=-1, length=1, width=1, height=1):
        ops.mesh.primitive_cube_add()
        context.active_object.name = reference
        size = (length, width, height)
        ops.transform.resize(size)
        return reference        

    @mod3D_api(reference = defined_object, frame = positive_int)
    def move_to(self, reference, x=None, y=None, z=None, frame=1, immediate=False):	        
        o = data.objects[reference]
        context.scene.frame_set(frame=frame)
        if immediate:
          o.keyframe_insert('location', frame=frame - 1)
    
        if (x != None):
            o.location.x = x
        if (y != None):
            o.location.y = y
        if (z != None):
            o.location.z = z
                
        o.keyframe_insert('location', frame=frame)
            
        return reference

    @mod3D_api(reference = defined_object, frame = positive_int)
    def scale(self, reference, x=None, y=None, z=None, frame=1, immediate=False):	        
        o = data.objects[reference]
        context.scene.frame_set(frame=frame)
        if immediate:
          o.keyframe_insert('scale', frame=frame - 1)
    
        if (x != None):
            o.scale.x = x
        if (y != None):
            o.scale.y = y
        if (z != None):
            o.scale.z = z
                
        o.keyframe_insert('scale', frame=frame)
            
        return reference

    @mod3D_api(reference = defined_material, frame = positive_int)
    def set_material_property(self, reference, prop, value, immediate=True, frame=1):
        context.scene.frame_set(frame=frame)
        o = data.materials[reference]

        if immediate:
            o.keyframe_insert(prop, frame=frame - 1)

        setattr(o, prop, value)
                
        o.keyframe_insert(prop, frame=frame)
            
        return reference
    

    @dbus.service.method(dbus_interface='de.tuberlin.uebb.modelica3d.api',
                         in_signature='a{sv}', 
                         out_signature='i')
    def make_cone(self, parameters):	
        bpy.ops.mesh.primitive_cone_add(radius=parameters['diameter'] / 2.0, depth=parameters['height'])
        bpy.context.active_object.name = str(parameters['reference'])
        return 0

    @dbus.service.method(dbus_interface='de.tuberlin.uebb.modelica3d.api',
                         in_signature='a{sv}', 
                         out_signature='i')
    def make_sphere(self, parameters):	
        bpy.ops.mesh.primitive_sphere_add(size=parameters['size'])
        bpy.context.active_object.name = str(parameters['reference'])
        return 0

    @dbus.service.method(dbus_interface='de.tuberlin.uebb.modelica3d.api',
                         in_signature='a{sv}', 
                         out_signature='i')
    def make_cylinder(self, parameters):	
        bpy.ops.mesh.primitive_cone_add(radius=parameters['diameter'] / 2.0, depth=parameters['height'])
        bpy.context.active_object.name = str(parameters['reference'])
        return 0
    

    _MATRIX_ARGS=map(lambda x : "R_" + str(x[0]) + "_" + str(y[1]),product(range(1,4),repeat=2))
    @dbus.service.method(dbus_interface='de.tuberlin.uebb.modelica3d.api',
                         in_signature='a{sv}', 
                         out_signature='i')
    def rotate(self, parameters):
        name = str(parameters['reference'])
        o = bpy.data.objects[name]
        bpy.context.scene.objects.active=o
        r = list(tee(map(lambda x : parameters[x], _MATRIX_ARGS),3))
        print("Rot: %s" % str(r))
        m = Matrix(r)
        e = m.to_euler()        
        bpy.ops.transform.translate(value=e[0], axis=(1.0,0,0))
        bpy.ops.transform.translate(value=e[1], axis=(0,1.0,0))
        bpy.ops.transform.translate(value=e[2], axis=(0,0,1.0))

if __name__ == '__main__':

    session_bus = dbus.SessionBus()
    name = dbus.service.BusName("de.tuberlin.uebb.modelica3d.server", session_bus)
    api = Modelica3DAPI(session_bus, "/de/tuberlin/uebb/modelica3d/server")    
    l.run()
