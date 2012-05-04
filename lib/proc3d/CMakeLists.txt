find_package(Boost REQUIRED)

add_definitions(-std=c++0x)

include_directories(${Boost_INCLUDES})
set(proc3d_src "${CMAKE_SOURCE_DIR}/lib/proc3d/src/")

add_library(proc3d "${proc3d_src}/proc3d.cpp")

