set(app_name "ARAppOsgViewer")

include_directories(${OSGAUDIO_INCLUDE_DIR})

file(GLOB app_srcs
	*.cpp
)


# create an executable
if (APPLE)
	set(platform MACOSX_BUNDLE)
endif (APPLE)

add_executable(${app_name} ${platform} ${app_srcs})

# link external libs
target_link_libraries(${app_name} 
	${OPENTHREADS_LIBRARY}
	${OPENSCENEGRAPH_LIBRARIES}
	${OPENGL_LIBRARIES}
	AREngine
)

install_arengine(ARAppOsgViewer)