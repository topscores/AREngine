set(app_name "ARAppOsgViewer")

include_directories(${OSGAUDIO_INCLUDE_DIR})

file(GLOB app_srcs
	*.cpp
)


# create an executable
add_executable(${app_name} ${app_srcs})

# link external libs
target_link_libraries(${app_name} 
	${OPENTHREADS_LIBRARY}
	${OPENSCENEGRAPH_LIBRARIES}
	${OPENGL_LIBRARIES}
	AREngine
)

install_arengine(ARAppOsgViewer)