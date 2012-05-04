set(mod3d_src "${CMAKE_SOURCE_DIR}/lib/mod3d/src/")

if(USE_OMC)
  # Modelica library to install
  install(DIRECTORY "${mod3d_src}/modelica/Modelica3D"
	DESTINATION "${OMC_MOD_LIB_DIR}/ModelicaServices 1.0")
endif(USE_OMC)
