#
# Dive into the subdirectories
#

include_directories(
	${CMAKE_SOURCE_DIR}/include 
)

#set (DIRS ARAppOsgViewer ARAppWX AREngine AREngineWX)
set (DIRS AREngine)

foreach (DIR ${DIRS})
	add_subdirectory(${DIR})
endforeach (DIR ${DIRS})