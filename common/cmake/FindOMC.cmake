# Tries to detect omc modelica-library and include dirs, does not search for binary yet

# This will also search ${CMAKE_PREFIX_PATH}/include automagically
find_path(OMC_INCLUDE_DIR modelica.h PATH_SUFFIXES omc)

# This will _not_ search ${CMAKE_PREFIX_PATH}/lib automagically, we need to give a search hint
find_path(OMC_MOD_LIB_DIR "Modelica 3.1/package.mo" 
  PATHS "${CMAKE_LIBRARY_PATH}/omlibrary"
        "${CMAKE_PREFIX_PATH}/lib/omlibrary" /usr/lib/omlibrary)

# This will also search ${CMAKE_PREFIX_PATH}/lib automagically
find_library(OMC_RUNTIME omcruntime PATH_SUFFIXES omc)

if ( OMC_INCLUDE_DIR AND OMC_MOD_LIB_DIR AND OMC_RUNTIME)
  set( OMC_FOUND TRUE )
endif( OMC_INCLUDE_DIR AND OMC_MOD_LIB_DIR AND OMC_RUNTIME)

if( OMC_INCLUDE_DIR)
  set( OMC_INCLUDES "${OMC_INCLUDE_DIR}")
endif ( OMC_INCLUDE_DIR )

if( OMC_RUNTIME)
  get_filename_component(OMC_LIBRARY_DIR ${OMC_RUNTIME} PATH)
endif( OMC_RUNTIME)

if( OMC_FOUND )
    message( STATUS "Found omc: ${OMC_RUNTIME}" )
    else( OMC_FOUND )
    if( OMC_FIND_REQUIRED )
    	message( FATAL_ERROR "Could not find the openmodelica installation" )
    endif( OMC_FIND_REQUIRED )
endif( OMC_FOUND )


