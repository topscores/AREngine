#
# Install product from each target
#

macro(install_arengine target)

	if (WIN32)
		install(
			TARGETS ${target}
			RUNTIME DESTINATION bin
			LIBRARY DESTINATION lib
			ARCHIVE DESTINATION lib
		)
	endif (WIN32)

endmacro(install_arengine target)