@echo off
SetLocal EnableDelayedExpansion
(set PATH=R:\development\libs\qt\qt.5.7.0.static.x32\qtbase\lib;!PATH!)
if defined QT_PLUGIN_PATH (
    set QT_PLUGIN_PATH=R:\development\libs\qt\qt.5.7.0.static.x32\qtbase\plugins;!QT_PLUGIN_PATH!
) else (
    set QT_PLUGIN_PATH=R:\development\libs\qt\qt.5.7.0.static.x32\qtbase\plugins
)
R:\development\libs\qt\qt.5.7.0.static.x32\qtbase\bin\uic.exe %*
EndLocal
