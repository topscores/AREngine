set(app_name "mkhash")


file(GLOB app_srcs
	*.cpp
)


# create an executable
if (APPLE)
	set(platform MACOSX_BUNDLE)
endif (APPLE)

add_definitions(-DUNICODE -D_UNICODE)
add_executable(${app_name} ${platform} ${app_srcs})

# link external libs
target_link_libraries(${app_name} 
	AREngine
)

install_arengine(mkhash)