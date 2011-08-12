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