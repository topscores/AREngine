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
AREngine is a library 1.0.

Build AREngine.
---------------

Required Packages.
-- Visual Studio .NET 2003
-- ARTOOLKIT 2.72.1 (Larngear Branch)
-- OSG 2.8.5
-- OSGART 2.0.1 (Larngear Branch)
-- WXWIDGET 2.8.12
-- SDL-1.2.15
-- SDL_mixer-1.2.12
-- Boost-1.4.8

Config file format.
-------------------

Changes log.
------------------------
version 1.0
- Support vdo action
- Check avaiability of catpure device before starting program
- Sync Animation
- Support Multiple Camera
- Support Sound

version 0.811
- Support AppConf tag (Application specific configurations)
- Move osgdb_osgAudio to same location as other plugins

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
