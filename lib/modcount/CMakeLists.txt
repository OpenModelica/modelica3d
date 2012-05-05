
set(modcount_src "${CMAKE_SOURCE_DIR}/lib/modcount/src/")

# modcount c-library
add_library(modcount "${modcount_src}/c/modcount.c")

add_definitions(-fPIC)

if(USE_OMC)
  # Modelica library to install
  install(DIRECTORY "${modcount_src}/modelica/modcount"
	DESTINATION "${OMC_MOD_LIB_DIR}/ModelicaServices 1.0")

  # Install library header
  install(FILES "${modcount_src}/c/modcount.h" DESTINATION ${OMC_INCLUDE_DIR})

  install(TARGETS modcount  
    LIBRARY DESTINATION ${OMC_LIBRARY_DIR}
    ARCHIVE DESTINATION ${OMC_LIBRARY_DIR}
  )
endif(USE_OMC)
