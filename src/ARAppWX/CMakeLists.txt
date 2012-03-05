set(app_name "ARAppWX")

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SDL_INCLUDE_DIR})

include_directories(${wxWidgets_INCLUDE_DIRS})
include(${wxWidgets_USE_FILE})

file(GLOB app_srcs
	*.cpp
)


# create an executable
if (WIN32)
	set(platform WIN32)
endif (WIN32)

if (APPLE)
	set(platform MACOSX_BUNDLE)
endif (APPLE)

add_executable(${app_name} ${platform} ${app_srcs})

# link external libs
target_link_libraries(${app_name} 
	${OPENTHREADS_LIBRARY}
	${OPENSCENEGRAPH_LIBRARIES}
	${OPENGL_LIBRARIES}
	${wxWidgets_LIBRARIES}
	${Boost_LIBRARIES}
	AREngine
	AREngineWX
)

install_arengine(ARAppWX)
