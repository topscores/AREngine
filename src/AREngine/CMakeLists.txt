set(lib_name "AREngine")

include_directories(${SDL_INCLUDE_DIR})
include_directories(${SDLMIXER_INCLUDE_DIR})

file(GLOB lib_srcs
	*.cpp
)

file(GLOB lib_hdrs
	${CMAKE_SOURCE_DIR}/include/AREngine/*.h
)

set(lib_export 	
	${CMAKE_SOURCE_DIR}/include/AREngine/Export
)

add_definitions(-DUNICODE -D_UNICODE)

# create a shared library - always
add_library(${lib_name} SHARED ${lib_srcs} ${lib_hdrs} ${lib_export})

# we are going to build a lib - tell the preprocessor
set_target_properties(AREngine 
			PROPERTIES
			DEFINE_SYMBOL ARENGINE_LIBRARY
			VERSION ${ARENGINE_VERSION_MINOR}
)

# link external libs
target_link_libraries(${lib_name} 
	${OPENTHREADS_LIBRARY}
	${OPENSCENEGRAPH_LIBRARIES}
	${OPENGL_LIBRARIES}
	${OSGART_LIBRARY}
	${SDL_LIBRARY}
	${SDLMIXER_LIBRARY}
)

install(FILES ${lib_hdrs} 
		DESTINATION include/${lib_name}
)

install_arengine(AREngine)