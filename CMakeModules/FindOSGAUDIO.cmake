# This module defines
# OSGAUDIO_LIBRARY
# OSGAUDIO_FOUND, if false, do not try to link to ode 
# OSGAUDIO_INCLUDE_DIR, where to find the headers
#
# $OSGAUDIO_DIR is an environment variable that would
# correspond to the ./configure --prefix=$OSGAUDIO_DIR
#
# based on the FindODE.cmake file Created by David Guthrie. Based on code by Robert 
# Osfield 

FIND_PATH(OSGAUDIO_INCLUDE_DIR SoundManager osgAudio/SoundManager.h
    ${OSGAUDIO_DIR}/include
    $ENV{OSGAUDIO_DIR}/include
    $ENV{OSGAUDIO_DIR} 
	$ENV{CSPDEVPACK}/include
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include
    /usr/include
    /sw/include # Fink
    /opt/local/include # DarwinPorts
    /opt/csw/include # Blastwave
    /opt/include
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OSG_ROOT]/include
    /usr/freeware/include
)


MACRO(FIND_OSGAUDIO_LIBRARY MYLIBRARY MYLIBRARYNAME)

FIND_LIBRARY(${MYLIBRARY}
    NAMES ${MYLIBRARYNAME}
    PATHS
    ${OSGAUDIO_DIR}/lib
    $ENV{OSGAUDIO_DIR}/lib
    $ENV{OSGAUDIO_DIR}
    $ENV{CSPDEVPACK}/lib
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/lib
    /usr/lib
    /sw/lib
    /opt/local/lib
    /opt/csw/lib
    /opt/lib
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OSG_ROOT]/lib
    /usr/freeware/lib64
)

ENDMACRO(FIND_OSGAUDIO_LIBRARY MYLIBRARY MYLIBRARYNAME)

SET(RELEASE_LIST osgAudio)
FIND_OSGAUDIO_LIBRARY(OSGAUDIO_LIBRARY osgAudio)

SET(DEBUG_LIST osgAudiod)
FIND_OSGAUDIO_LIBRARY(OSGAUDIO_LIBRARY_DEBUG osgAudiod)

SET(OSGAUDIO_FOUND "NO")
IF(OSGAUDIO_LIBRARY AND OSGAUDIO_INCLUDE_DIR)
    SET(OSGAUDIO_FOUND "YES")
ENDIF(OSGAUDIO_LIBRARY AND OSGAUDIO_INCLUDE_DIR)