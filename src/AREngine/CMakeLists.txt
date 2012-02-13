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
set_target_properties(${lib_name} 
			PROPERTIES
			DEFINE_SYMBOL ARENGINE_LIBRARY
)

if (WIN32)
	set (PLATFORM_DEPENDENT_LIBRARY
		strmiids.lib)
endif (WIN32)

# link external libs
target_link_libraries(${lib_name} 
	${OPENTHREADS_LIBRARY}
	${OPENSCENEGRAPH_LIBRARIES}
	${OPENGL_LIBRARIES}
	${OSGART_LIBRARY}
	${SDL_LIBRARY}
	${SDLMIXER_LIBRARY}
	${PLATFORM_DEPENDENT_LIBRARY}
)

install(FILES ${lib_hdrs} 
		DESTINATION include/${lib_name}
)

install(FILES ${lib_export} 
		DESTINATION include/${lib_name}
)

install_arengine(AREngine)

#
# Optional SDK packaging (default is on)
#
option(ARENGINE_BUILD_SDK "Build as an SDK which will copy all necessary files into the AREngine folder" ON)
if(ARENGINE_BUILD_SDK)
	# OSG
	install(
		DIRECTORY "${OSG_INCLUDE_DIR}/../"
		DESTINATION "."
	)

	# OSGART
	install(
		DIRECTORY "${OSGART_INCLUDE_DIR}/../"
		DESTINATION "."
	)

	if(WIN32)		
		# SDL
		install(
			DIRECTORY "${SDL_INCLUDE_DIR}"
			DESTINATION "."
		)

		file(GLOB sdllibs
			${SDL_INCLUDE_DIR}/../lib/*.lib
		)
		install(
			FILES ${sdllibs}
			DESTINATION "lib"
		)

		file(GLOB sdldlls
			${SDL_INCLUDE_DIR}/../lib/*.dll
		)
		install(
			FILES ${sdldlls}
			DESTINATION "bin"
		)

		# SDLMIXER
		install(
			DIRECTORY "${SDLMIXER_INCLUDE_DIR}"
			DESTINATION "."
		)

		file(GLOB sdlmixerlibs
			${SDLMIXER_INCLUDE_DIR}/../lib/*.lib
		)
		install(
			FILES ${sdlmixerlibs}
			DESTINATION "lib"
		)

		file(GLOB sdlmixerdlls
			${SDLMIXER_INCLUDE_DIR}/../lib/*.dll
		)
		install(
			FILES ${sdlmixerdlls}
			DESTINATION "bin"
		)

	else(WIN32)
		if (APPLE)
			# SDL_INCLUDE_DIR = Library/Frameworks/SDL.framework/Versions/Current/Headers
			install(
				DIRECTORY "${SDL_INCLUDE_DIR}/../../../"
				DESTINATION "framework/SDL.framework"
			)
			install(
				DIRECTORY "${SDLMIXER_INCLUDE_DIR}/../../../"
				DESTINATION "framework/SDL_mixer.framework"
			)
		endif (APPLE)
	endif(WIN32)
endif(ARENGINE_BUILD_SDK)



