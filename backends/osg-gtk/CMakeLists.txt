find_package(GtkGl REQUIRED)
find_package(OpenSceneGraph REQUIRED osgGA osgText osgViewer osgDB)

if(MINGW)
add_definitions(-std=c++0x -U__STRICT_ANSI__ -mms-bitfields)
else(MINGW)
add_definitions(-std=c++0x)
endif(MINGW)

#requires osg, gtk and proc3d
include_directories(${Boost_INCLUDE_DIR} ${GTK_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/lib/proc3d/src/" ${GTKGL_INCLUDE_DIRS} ${OPENSCENEGRAPH_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS} ${GTKGL_LIBRARY_DIRS})

set(osg-gtk_src "${CMAKE_SOURCE_DIR}/backends/osg-gtk/src/")

add_library(m3d-osg-gtk SHARED
	"${osg-gtk_src}/osg_interpreter.hpp"
	"${osg-gtk_src}/osgviewerGTK.hpp"
	"${osg-gtk_src}/osgviewerGTK.cpp"
	"${osg-gtk_src}/osggtkdrawingarea.h"
	"${osg-gtk_src}/osggtkdrawingarea.cpp"
	)
add_dependencies(m3d-osg-gtk proc3d)

target_link_libraries(m3d-osg-gtk ${OPENSCENEGRAPH_LIBRARIES} ${GTK_LIBRARIES} ${GTKGL_LIBRARIES} proc3d)

add_executable(viewer "${osg-gtk_src}/viewer.cpp")
target_link_libraries(viewer m3d-osg-gtk)

install(TARGETS m3d-osg-gtk
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(FILES "python/dbus-server.py"
	DESTINATION "lib/omlibrary-modelica3d/osg-gtk")
