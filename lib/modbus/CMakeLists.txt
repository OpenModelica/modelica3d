find_package(DBUS REQUIRED)

include_directories(${DBUS_INCLUDES} ${OMC_INCLUDES})
set(modbus_src "${CMAKE_SOURCE_DIR}/lib/modbus/src/")

add_library(modbus "${modbus_src}/c/modbus.c")

if (USE_OMC)
  # Modelica library to install
  install(DIRECTORY "${modbus_src}/modelica/modbus"
	DESTINATION "${OMC_MOD_LIB_DIR}/ModelicaServices 1.0")

  # Install library header
  install(FILES "${modbus_src}/c/modbus.h" DESTINATION ${OMC_INCLUDE_DIR})

  install(TARGETS modbus
    LIBRARY DESTINATION ${OMC_LIBRARY_DIR}
    ARCHIVE DESTINATION ${OMC_LIBRARY_DIR}
  )
endif(USE_OMC)
