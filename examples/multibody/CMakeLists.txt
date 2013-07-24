# Modelica library to install
set(multibody_src "${CMAKE_SOURCE_DIR}/examples/multibody/src/modelica/")

install(FILES 
	"${multibody_src}/Internal.mo"
	"${multibody_src}/Pendulum.mo"
	"${multibody_src}/DoublePendulum.mo"
	"${multibody_src}/Engine1b.mo"
	DESTINATION "lib/omlibrary-modelica3d")
