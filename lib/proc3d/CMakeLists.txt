find_package(Boost REQUIRED)

if(MINGW)
add_definitions(-std=c++0x -fPIC -U__STRICT_ANSI__)
else(MINGW)
add_definitions(-std=c++0x -fPIC)
endif(MINGW)

include_directories(${Boost_INCLUDE_DIR} ${Boost_INCLUDE_DIR}/boost)
set(proc3d_src "${CMAKE_SOURCE_DIR}/lib/proc3d/src/")

add_library(proc3d SHARED "${proc3d_src}/proc3d.cpp")

install(TARGETS proc3d
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
