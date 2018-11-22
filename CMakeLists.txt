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

set(RESOURCES
    resource.qrc
)

qt5_add_resources(RSC ${RESOURCES})

add_executable(QVPaint ${HEADERS} ${SOURCES} ${RSC})
target_link_libraries(QVPaint Qt5::Core Qt5::Gui Qt5::Widgets)
