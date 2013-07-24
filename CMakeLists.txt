cmake_minimum_required(VERSION 2.8)

project(modelica3d_dbus CXX C)
option(USE_OMC "use openmodelica" ON)
option(OSG_BACKEND "build openscenegraph backed" ON)
option(INSTALL_EXAMPLES "install examples" ON)
option(BLENDER_BACKEND "build blender backed" OFF)
set(MODELICA_SERVICES_LIBRARY "ModelicaServices 3.2.1 modelica3d" CACHE STRING "Modelica Services library name")

set(CPACK_PACKAGE_CONTACT "openmodelica@ida.liu.se")
set(CPACK_SET_DESTDIR "ON")
include(CPack)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/common/cmake")

if (USE_OMC)
  find_package(OMC REQUIRED)
endif(USE_OMC)

include_directories(${OMC_INCLUDES})

add_subdirectory(lib/modcount)
add_subdirectory(lib/modbus)
add_subdirectory(lib/mod3d)
add_subdirectory(lib/proc3d)

if(OSG_BACKEND)
  add_subdirectory(backends/osg-gtk)
endif(OSG_BACKEND)

if(BLENDER_BACKEND)
  add_subdirectory(backends/blender-2.59)
endif(BLENDER_BACKEND)

if(INSTALL_EXAMPLES)
  add_subdirectory(examples/multibody)
endif(INSTALL_EXAMPLES)
