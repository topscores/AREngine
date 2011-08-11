set(lib_name "AREngine")

file(GLOB lib_srcs
	*.cpp
)

file(GLOB lib_hdrs
	${CMAKE_SOURCE_DIR}/include/AREngine/*.h
)

set(lib_export 	
	${CMAKE_SOURCE_DIR}/include/AREngine/Export
)

# create a shared library - always
add_library(${lib_name} SHARED ${lib_srcs} ${lib_hdrs} ${lib_export})

# we are going to build a lib - tell the preprocessor
add_definitions(-DARENGINE_LIBRARY)

# link external libs
target_link_libraries(${lib_name} 
	${OPENTHREADS_LIBRARY}
	${OSG_LIBRARY} 
	${OSGDB_LIBRARY}
	${OSGUTIL_LIBRARY}
	${OSGGA_LIBRARY}
	${OSGVIEWER_LIBRARY}
	${OSGTEXT_LIBRARY}
	${OPENGL_LIBRARY} 
	${GLUT_LIBRARY}
	${OSGART_LIBRARY}
	${OSGAUDIO_LIBRARY}
)