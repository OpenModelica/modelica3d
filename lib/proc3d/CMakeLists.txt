find_package(Boost REQUIRED)

add_definitions(-std=c++0x -fPIC)

include_directories(${Boost_INCLUDES})
set(proc3d_src "${CMAKE_SOURCE_DIR}/lib/proc3d/src/")

add_library(proc3d SHARED "${proc3d_src}/proc3d.cpp")

install(TARGETS proc3d
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
