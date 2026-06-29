@echo off
SetLocal EnableDelayedExpansion
(set QT_VERSION=5.7.0)
(set QT_VER=5.7)
(set QT_VERSION_TAG=570)
(set QT_INSTALL_DOCS=R:/development/libs/qt/qt.5.7.0.static.x32/qtbase/doc)
R:\development\libs\qt\qt.5.7.0.static.x32\qtbase\bin\qdoc.exe %*
EndLocal
