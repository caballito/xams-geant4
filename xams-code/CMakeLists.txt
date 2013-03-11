#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(name xams_sim)
project(${name})

set(CMAKE_MODULE_PATH "/home/caballito/simulation/geant4/geant4.9.5.p01/cmake/Modules" )


#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
	find_package(Geant4 REQUIRED ui_all vis_all)
else()
	find_package(Geant4 REQUIRED)
endif()


# Find ROOT (required package)
#
#find_package(ROOT REQUIRED)
find_package(ROOT QUIET)
if(NOT ROOT_FOUND)
	message(STATUS "G4 Examples: ROOT package not found. --> Application disabled")  
	return()
endif()


#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIR} ${ROOT_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include ${GEANT4_INCLUDE_DIR} ${ROOT_INCLUDE_DIR})


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
#add_executable(${name} EXCLUDE_FROM_ALL ${name}.cc ${sources})
add_executable(${name} ${name}.cc ${sources} ${headers})
target_link_libraries(${name} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})


#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(XAMS_SCRIPTS
	run/preinit.mac
	run/runit.mac
	run/runit_onlyXe.mac
	run/visualize.mac
)

#message ("NvB ${PROJECT_SOURCE_DIR}")
#message ("NvB ${XAMS_SCRIPTS}")

foreach(_script ${XAMS_SCRIPTS})
	configure_file(
		${PROJECT_SOURCE_DIR}/${_script}
		${PROJECT_BINARY_DIR}/${_script}
		COPYONLY
	)
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
#add_custom_target(nvb_xams_v1.0 DEPENDS ${name})

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS ${name} DESTINATION bin)


