#
#   Digital Clock: window on top plugin
#   Copyright (C) 2014-2020  Nick Korotysh <nick.korotysh@gmail.com>
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

#-------------------------------------------------
#
# Project created by QtCreator 2014-01-02T22:55:42
#
#-------------------------------------------------

QT       += core

include(../../common.pri)

TARGET = win_on_top

SOURCES += win_on_top.cpp

HEADERS += win_on_top.h

include(lang/lang.pri)
include(../../../qm_gen.pri)

OTHER_FILES += win_on_top.json

win32:RC_FILE = win_on_top.rc

win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../plugin_core/debug/
else:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../plugin_core/release/
win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../clock_common/debug/
else:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../clock_common/release/
