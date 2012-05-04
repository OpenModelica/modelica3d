# - Try to find libglfw
# Once done this will define
#  LIBGLFW_FOUND - System has LibXml2
#  LIBGLFW_INCLUDE_DIRS - The LibXml2 include directories
#  LIBGLFW_LIBRARIES - The libraries needed to use LibXml2

find_path(LIBGLFW_INCLUDE_DIR GL/glfw.h)

find_library(LIBGLFW_LIBRARY NAMES glfw libglfw)

set(LIBGLFW_LIBRARIES ${LIBGLFW_LIBRARY} )
set(LIBGLFW_INCLUDE_DIRS ${LIBGLFW_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libglfw  DEFAULT_MSG
                                  LIBGLFW_LIBRARY LIBGLFW_INCLUDE_DIR)

mark_as_advanced(LIBGLFW_INCLUDE_DIR LIBGLFW_LIBRARY)