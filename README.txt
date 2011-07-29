Read me for AREngine.
=====================

Contents.
---------

About this archive.
Build AREngine.
Config file format.
Changes log.
Know issues in this release.


About this archive.
--------------------
AREngine is a library xxx.

Build AREngine.
---------------

Required Packages.
-- Visual Studio 9.0
-- ARTOOLKIT 2.72
-- OSG 2.8.4
-- OSGART 2.0RC3
-- WXWIDGET 2.8.12
-- osgAudio 2.0.0
-- openal 1.1
-- freealut-1.1.0
-- libogg-1.2.2
-- libvorbis-1.3.2

Currently solution file is provided for VS9.0SP1. 
In order to build AREngine you need to set following environment variable properly
-- OSG_ROOT : Root directory of OpenSceneGraph (normally /Program File/OpenSceneGraph)
-- OSGART_ROOT : Root directory of osgART (normally /Program File/osgART)
-- WxWIDGET_ROOT : Root directory of wxWidget (normally /Program File/wxWidget)
-- AL_ROOT : Root directory of OpenAL
-- ALUT_ROOT : Root directory of AL Utility toolkit
-- OSGAUDIO_ROOT : Root directory of osgAudio
-- LIBOGG_ROOT : Root directory of libogg
-- LIBVORBIS_ROOT : Root directory of libvorbis


Config file format.
-------------------

Changes log.
------------------------

version 0.81
- Add support for Sound Action
- Can remove all objects on a marker
- Can remove all huds on a scene
- Change matrix manipulation from around fixed axis to local axis
- Support tracking KeyDown event from keycode value
- Callback can be added before and after action
- Add TwistIn transition
- Regulate callback event checking frequency default to 25 time/sec

version 0.80
- Conditional Callback
- Collision checker
- KeyDown checker
- Add/Remove action for models and huds

Known issues in this release.
-----------------------------
