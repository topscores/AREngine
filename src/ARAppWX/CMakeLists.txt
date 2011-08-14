set(app_name "ARAppWX")

include_directories(${OSGAUDIO_INCLUDE_DIR})

include_directories(${wxWidgets_INCLUDE_DIRS})
include(${wxWidgets_USE_FILE})

file(GLOB app_srcs
	*.cpp
)


# create an executable
if (WIN32)
	add_executable(${app_name} WIN32 ${app_srcs})
else (WIN32)
	add_executable(${app_name} ${app_srcs})
endif (WIN32)

# link external libs
target_link_libraries(${app_name} 
	${OPENTHREADS_LIBRARY}
	${OPENSCENEGRAPH_LIBRARIES}
	${OPENGL_LIBRARIES}
	${wxWidgets_LIBRARIES}
	AREngine
	AREngineWX
)

install_arengine(ARAppWX)
