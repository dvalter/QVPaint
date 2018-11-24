# Copyright (C) 2017-2018 Dmitry Valter
#
# QVPaint is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License, version 3
#  or the GNU Lesser General Public License, version 3 as published
#  by the Free Software Foundation or either under the terms of the
#  Do What the Fuck You Want to Public License, Version 2 with
#  The No Warranty Exception 1.0.
#
#  QVPaint is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License,  for more details.
#
#  You should have received a copy of the GNU General Public License,
#  GNU Lesser General Public License and Do What the Fuck You
#  Want to Public License along with QVPaint. If not, see
#  <https://www.gnu.org/licenses/> and <http://www.wtfpl.net>.

cmake_minimum_required(VERSION 3.6)
project(QVPaint)


set(CMAKE_CXX_STANDARD 17)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Gui_EXECUTABLE_COMPILE_FLAGS} -std=c++17 -Wall -Wextra -O2" )
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -fprofile-arcs -ftest-coverage -lgcov")

find_package(Qt5 COMPONENTS Core Gui Widgets)


include_directories(${Qt5Gui_INCLUDES})
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Core_INCLUDES})

add_definitions(${Qt5Gui_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Core_DEFINITIONS})

include_directories(.)


set(SOURCES
        main.cpp
        qvpmainwindow.cpp
        qvpdocument.cpp
        qvpshape.cpp
        qvpdot.cpp
        qvpline.cpp
        qvpellipse.cpp
        qvpellipticarc.cpp
        qvpaction.cpp
        qvprasterizedshape.cpp
        qvpshapeactions.cpp
        )

set(HEADERS
        qvpmainwindow.h
        qvpdocument.h
        qvpshape.h
        config.h
        qvpdot.h
        qvpline.h
        qvpellipse.h
        qvpellipticarc.h
        qvpaction.h
        qvprasterizedshape.h
        qvpshapeactions.h
)

# uncomment for properly licensed icons
#set(RESOURCES
#    resource.qrc
#)
#
#qt5_add_resources(RSC ${RESOURCES})


add_executable(QVPaint ${HEADERS} ${SOURCES} ${RSC})
target_link_libraries(QVPaint Qt5::Core Qt5::Gui Qt5::Widgets)

add_custom_command(TARGET QVPaint PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/imgs $<TARGET_FILE_DIR:QVPaint>/imgs)
