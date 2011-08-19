#
# Install product from each target
#

macro(install_arengine target)

	install(
		TARGETS ${target}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		BUNDLE DESTINATION bin
	)

endmacro(install_arengine target)