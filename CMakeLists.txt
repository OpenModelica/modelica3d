project(modelica3d_dbus CXX C)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/common/cmake")

if (USE_OMC)
  find_package(OMC REQUIRED)
endif(USE_OMC)

include_directories(${OMC_INCLUDES})

subdirs(lib/modcount lib/modbus lib/mod3d lib/proc3d)

if(OSG_BACKEND)
  subdirs(backends/osg-gtk)
endif(OSG_BACKEND)

if(INSTALL_EXAMPLES)
  subdirs(examples/multibody)
endif(INSTALL_EXAMPLES)
