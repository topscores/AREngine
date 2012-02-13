#
# Dive into the subdirectories
#

include_directories(
	${CMAKE_SOURCE_DIR}/include 
)

set (DIRS AREngine AREngineWX ARAppOsgViewer ARAppWX mkhash)

foreach (DIR ${DIRS})
	add_subdirectory(${DIR})
endforeach (DIR ${DIRS})