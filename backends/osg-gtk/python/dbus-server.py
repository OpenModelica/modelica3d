
import os
import sys
import platform
from ctypes import *

from dbus.mainloop.glib import DBusGMainLoop
import dbus
import dbus.service

DBusGMainLoop(set_as_default=True)

from gi.repository import GObject

l = GObject.MainLoop()

# Wrapper for dbus api decorator
dec = dbus.service.method(dbus_interface='de.tuberlin.uebb.modelica3d.api',
                          in_signature='a{sv}',
                          out_signature='s')

# decorate a function with optional typechecks
def mod3D_api(**checks):
    def tc(f):
        #print("Decorating %s with %s" % (str(f), checks))

        def checked(s, **p) :    
            #print("Got: %s %s" % (str(s), str(p)))
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
    return (True, "%s is already in use." % str(ref))

def defined_object(ref) :
    return (True, "%s undefined" % str(ref))

def undefined_material(ref) :
    return (True, "%s is already in use." % str(ref))

def defined_material(ref) :
    return (True, "%s undefined" % str(ref))

# Check for positive value
def positive(s):
    return (s <= 0.0, "expected positive value")

def positive_int(i):
    return (isinstance(i,int) and i >= 0, "expected positive int")

def not_zero(x):
    return (x != 0, "parameter may not be zero")

def existing_file(x):
    return (os.path.isfile(x), "File %s does not exist!" % x)

class Modelica3DAPI(dbus.service.Object):

    @mod3D_api()
    def stop(self):
        # Quit the dbus server
        l.quit()
        return "stopped"

    @mod3D_api(reference = undefined_object, length = not_zero)
    def make_box(self, reference, length=1, width=1, height=1, tx=0.0, ty=0.0, tz=1.0):
        self.omg.proc3d_create_box(self.ctxt, c_char_p(reference), 
                                   c_double(tx), c_double(ty), c_double(tz),
                                   c_double(width), c_double(length), c_double(height))
        return reference

    @mod3D_api(reference = undefined_object, height = not_zero, diameter = not_zero)
    def make_cone(self, reference, x=0.0, y=0.0, z=1.0, diameter=1, height=5):
        self.omg.proc3d_create_cone(self.ctxt, c_char_p(reference), c_double(x), c_double(y), c_double(z), c_double(height), c_double(diameter / 2.0));
        return reference

    @mod3D_api(reference = undefined_object, size = not_zero)
    def make_sphere(self, reference, size=1):
        self.omg.proc3d_create_sphere(self.ctxt, c_char_p(reference), c_double(size / 2.0));
        return reference

    @mod3D_api(reference = undefined_object, height = not_zero, diameter = not_zero)
    def make_cylinder(self, reference, x=0.0, y=0.0, z=1.0, diameter=1, height = 10):
        self.omg.proc3d_create_cylinder(self.ctxt, c_char_p(reference), c_double(x), c_double(y), c_double(z), c_double(height), c_double(diameter / 2.0))
        return reference

    @mod3D_api(reference = defined_object, frame = positive_int)
    def move_to(self, reference, x=0.0, y=0.0, z=0.0, frame=1, immediate=False):	        
        self.omg.proc3d_set_translation(self.ctxt, c_char_p(reference), c_double(x), c_double(y), c_double(z), c_uint(frame));
        return reference

    @mod3D_api(reference = defined_object, frame = positive_int)
    def scale(self, reference, x=0.0, y=0.0, z=0.0, frame=1, immediate=False):
        self.omg.proc3d_set_scale(self.ctxt, c_char_p(reference), c_double(x), c_double(y), c_double(z), c_uint(frame))
        return reference

    @mod3D_api(reference = undefined_material)
    def make_material(self, reference):
        self.omg.proc3d_create_material(self.ctxt, c_char_p(reference))
        return reference

    @mod3D_api(reference = defined_object, material=defined_material)
    def apply_material(self, reference, material):
        self.omg.proc3d_apply_material(self.ctxt, c_char_p(reference), c_char_p(material))
        return reference

    @mod3D_api(reference = defined_material, frame = positive_int)
    def set_material_property(self, reference, prop, value, immediate=True, frame=1):
        self.omg.proc3d_set_material_property(self.ctxt, c_char_p(reference), c_double(value), c_char_p(prop), c_uint(frame))
        return reference

    @mod3D_api(reference = defined_material, frame = positive_int)
    def set_ambient_color(self, reference, r=0.5, g=0.5, b=0.5, a=0, immediate=True, frame=1):
        self.omg.proc3d_set_ambient_color(self.ctxt, c_char_p(reference), c_double(r), c_double(g) , c_double(b) , c_double(a), c_uint(frame))
        return reference

    @mod3D_api(reference = defined_material, frame = positive_int)
    def set_diffuse_color(self, reference, r=0.5, g=0.5, b=0.5, a=0, immediate=True, frame=1):
        self.omg.proc3d_set_diffuse_color(self.ctxt, c_char_p(reference), c_double(r), c_double(g) , c_double(b) , c_double(a), c_uint(frame))
        return reference

    @mod3D_api(reference = defined_material, frame = positive_int)
    def set_specular_color(self, reference, r=0.5, g=0.5, b=0.5, a=0, immediate=True, frame=1):
        self.omg.proc3d_set_specular_color(self.ctxt, c_char_p(reference), c_double(r), c_double(g) , c_double(b) , c_double(a), c_uint(frame))
        return reference

    @mod3D_api(reference = defined_object, frame = positive_int)
    def rotate(self, reference, 
               R_1_1, R_1_2, R_1_3, 
               R_2_1, R_2_2, R_2_3, 
               R_3_1, R_3_2, R_3_3,                
               frame=1):
        self.omg.proc3d_set_rotation_matrix(self.ctxt, c_char_p(reference),
                  c_double(R_1_1), c_double(R_1_2), c_double(R_1_3), 
                  c_double(R_2_1), c_double(R_2_2), c_double(R_2_3), 
                  c_double(R_3_1), c_double(R_3_2), c_double(R_3_3), 
                  c_uint(frame))
        return reference
    
    @mod3D_api(fileName = existing_file)
    def loadFromFile(self, fileName):
        self.omg.proc3d_load_object(self.ctxt, c_char_p(reference))

if __name__ == '__main__':
    # load DLL or shared object
    if (platform.system() == 'Linux'):
        omg = CDLL("libm3d-osg-gtk.so")  # linux shared object
    elif (platform.system() == 'Windows'):
        print "Windows OS not tested yet..."	# eventually add Windows shared library
        sys.exit()
    elif (platform.system() == 'Darwin'):
        print "MacOS Darwin OS not tested yet..." # eventually add MacOS shared library
        sys.exit()

    session_bus = dbus.SessionBus()
    name = dbus.service.BusName("de.tuberlin.uebb.modelica3d.server", session_bus)
    api = Modelica3DAPI(session_bus, "/de/tuberlin/uebb/modelica3d/server")

    ctxt = omg.osg_gtk_alloc_context()
    api.omg = omg
    api.ctxt = ctxt

    print("Running dbus-server...")
    l.run()
    print("dbus server finished.")

    omg.proc3d_send_signal(ctxt, 1) # run viewer
    omg.osg_gtk_free_context(ctxt)
