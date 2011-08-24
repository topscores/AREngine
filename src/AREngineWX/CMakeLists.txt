set(lib_name "AREngineWX")

include_directories(${wxWidgets_INCLUDE_DIRS})
include(${wxWidgets_USE_FILE})

file(GLOB lib_srcs
	*.cpp
)

file(GLOB lib_hdrs
	${CMAKE_SOURCE_DIR}/include/AREngineWX/*.h
)

set(lib_export 	
	${CMAKE_SOURCE_DIR}/include/AREngineWX/Export
)

# create a shared library - always
add_library(${lib_name} SHARED ${lib_srcs} ${lib_hdrs} ${lib_export})

# we are going to build a lib - tell the preprocessor
set_target_properties(AREngineWX 
			PROPERTIES
			DEFINE_SYMBOL ARENGINEWX_LIBRARY
)

# link external libs
target_link_libraries(${lib_name} 
	${OPENTHREADS_LIBRARY}
	${OPENSCENEGRAPH_LIBRARIES}
	${OPENGL_LIBRARY} 
	${wxWidgets_LIBRARIES}
	AREngine
)

install(FILES ${lib_hdrs} 
		DESTINATION include/${lib_name}
)

install_arengine(AREngineWX)

#
# Optional SDK packaging (default is on)
#
if(WIN32)		
	option(ARENGINE_BUILD_SDK "Build as an SDK which will copy all necessary files into the AREngine folder" ON)
	if(ARENGINE_BUILD_SDK)		
		# wxWidgets
		install(
			DIRECTORY ${wxWidgets_INCLUDE_DIRS}
			DESTINATION "."
		)

		install(
			DIRECTORY ${wxWidgets_LIB_DIR}/${wxWidgets_CONFIGURATION}
			DESTINATION "lib"
		)

		file(GLOB wxlibs
			${wxWidgets_LIB_DIR}/*.lib
		)
		install(
			FILES ${wxlibs}
			DESTINATION "lib"
		)

		file(GLOB wxdlls
			${wxWidgets_LIB_DIR}/*.dll
		)
		install(
			FILES ${wxdlls}
			DESTINATION "lib"
		)
		install(
			FILES ${wxdlls}
			DESTINATION "bin"
		)

	endif(ARENGINE_BUILD_SDK)
endif(WIN32)
