# Modelica library to install
set(multibody_src "${CMAKE_SOURCE_DIR}/examples/multibody/src/modelica/")

install(FILES "${multibody_src}/Internal.mo"
	DESTINATION "${OMC_MOD_LIB_DIR}/Modelica 3.1/Utilities")

install(FILES "${multibody_src}/Pendulum.mo"
	DESTINATION "${OMC_MOD_LIB_DIR}/Modelica 3.1/Mechanics/MultiBody/Examples/Elementary/")

install(FILES "${multibody_src}/DoublePendulum.mo"
	DESTINATION "${OMC_MOD_LIB_DIR}/Modelica 3.1/Mechanics/MultiBody/Examples/Elementary/")

install(FILES "${multibody_src}/Engine1b.mo"
	DESTINATION "${OMC_MOD_LIB_DIR}/Modelica 3.1/Mechanics/MultiBody/Examples/Loops/")
