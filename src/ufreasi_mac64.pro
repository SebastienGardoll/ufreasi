##
 #  user-FRiendly Elemental dAta procesSIng (uFREASI)
 #  
 #  Copyright © 2012 Oualid Khelefi.
 #
 #  Authors : see AUTHORS.
 #
 #  This program is free software: you can redistribute it and/or modify
 #  it under the terms of the GNU Lesser General Public License as published by
 #  the Free Software Foundation, either version 3 of the License, or
 #  (at your option) any later version.
 #
 #  This program is distributed in the hope that it will be useful,
 #  but WITHOUT ANY WARRANTY; without even the implied warranty of
 #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #  GNU Lesser General Public License for more details.
 #
 #  You should have received a copy of the GNU Lesser General Public License
 #  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 #

#------------------------------------------------------
#
# YOU MAY CHANGE SOME PATH (SEE PLATEFORM TARGET RULES)
#
#------------------------------------------------------


QT += core gui

TARGET = ufreasi
TEMPLATE = app


SOURCES += main.cpp\
        application.cpp \
    element.cpp \
    parser.cpp \
    data.cpp \
    processing.cpp \
    qcdialog.cpp \
    displaying.cpp \
    solution.cpp \
    regline.cpp

HEADERS += application.h \
    element.h \
    parser.h \
    data.h \
    processing.h \
    qcdialog.h \
    displaying.h \
    solution.h \
    regline.h

FORMS += application.ui \
    qcdialog.ui

RESOURCES += \
    res.qrc

CONFIG += release

CONFIG += qwt

OBJECTS_DIR = obj
MOC_DIR      = moc
RCC_DIR = rcc
UI_DIR = ui
DESTDIR = bin


### PLATEFORM TARGET RULES ### 

mac {
   # For Mac OS X, QWT library must be installed as a framework file (qwt.framework) in
   # the /Library/Frameworks directory in order to macdeployqt could find it

   LIBS = -F/Library/Frameworks/qwt.framework -framework qwt
   INCLUDEPATH += /Library/Frameworks/qwt.framework/Headers/
   DEPENDPATH += /Library/Frameworks/qwt.framework/Headers/
   ICON = res/chimie.icns
}