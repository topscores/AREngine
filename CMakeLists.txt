# Make file for AREngine
# Modified from osgART-2.0.0-RC3 

cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

project(AREngine)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMakeModules)

# some include
include(AREngineUtil)

#
# Check if somebody wants to build in-tree
#
if   (${CMAKE_BINARY_DIR} EQUAL ${CMAKE_SOURCE_DIR})
	MESSAGE("In-source builds are discouraged!")
endif(${CMAKE_BINARY_DIR} EQUAL ${CMAKE_SOURCE_DIR})

if(WIN32)
	set(CMAKE_DEBUG_POSTFIX  "d")
endif(WIN32)

set(ARENGINE_VERSION_MAJOR 0)
set(ARENGINE_VERSION_MINOR 8)
set(ARENGINE_VERSION_PATCH 1)
set(ARENGINE_VERSION_TWEAK 1)

set(ARENGINE_VERSION_FULL ${ARENGINE_VERSION_MAJOR}.${ARENGINE_VERSION_PATCH}.${ARENGINE_VERSION_PATCH}.${ARENGINE_VERSION_TWEAK})

# Finding various packages
find_package(OpenGL REQUIRED)

find_package(OpenThreads REQUIRED)
find_package(OpenSceneGraph 2.8.4 REQUIRED
		osgDB
		osgUtil
		osgGA
		osgViewer
		osgText
)

find_package(OSGART REQUIRED)

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)

find_package(wxWidgets COMPONENTS 
		net 
		gl 
		core 
		base 
		adv
)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package( Boost 1.48 COMPONENTS thread date_time)

# Add additional include directories
include_directories(${OPENGL_INCLUDE_DIR})

include_directories(${OPENTHREADS_INCLUDE_DIR})
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

include_directories(${OSGART_INCLUDE_DIR})

# we might want to honor the actual OSG Version
include(parse_OSG_version)

# check the real version
check_osg_version(${OSG_INCLUDE_DIR}/osg/Version)

# Mac needs these for building the examples
if(APPLE)
    find_library(CARBON_LIBRARY Carbon REQUIRED)
    #find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(QUICKTIME_LIBRARY QuickTime REQUIRED)
endif(APPLE)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

install(
	FILES
	${CMAKE_SOURCE_DIR}/README.txt
	${CMAKE_SOURCE_DIR}/LICENSE.txt
	${CMAKE_SOURCE_DIR}/config-format.rtf
	DESTINATION "share/AREngine"
)

install(
	DIRECTORY
	${CMAKE_SOURCE_DIR}/bin
	DESTINATION "."
)

#
# Dive into the subdirectories
#
add_subdirectory(src)

#
# Uninstall
#
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
  
add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

#
# Optional packaging for AREngine
#
option(ARENGINE_BUILD_PACKAGE "Build distribution packages" OFF)
if   (ARENGINE_BUILD_PACKAGE)
	configure_file(
		${CMAKE_SOURCE_DIR}/CMakeModules/Packaging.cmake.in
		${CMAKE_BINARY_DIR}/.cpack/ARENGINEPackaging.cmake	
		@ONLY
	)		
	include(${CMAKE_BINARY_DIR}/.cpack/ARENGINEPackaging.cmake)			
endif (ARENGINE_BUILD_PACKAGE)