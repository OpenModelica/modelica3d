set(mod3d_src "${CMAKE_SOURCE_DIR}/lib/mod3d/src/")

if(USE_OMC)
  # Modelica library to install
  install(FILES "${mod3d_src}/modelica/Modelica3D\ 3.2.1/package.mo"
	  DESTINATION "${OMC_MOD_LIB_DIR}/${MODELICA_SERVICES_LIBRARY}/Modelica3D")
  install(FILES "${mod3d_src}/modelica/package.mo"
	  DESTINATION "${OMC_MOD_LIB_DIR}/${MODELICA_SERVICES_LIBRARY}")
endif(USE_OMC)
