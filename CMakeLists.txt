cmake_minimum_required(VERSION 3.5)

project(GStreamerMSVC VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GSTREAMER_ROOT "D:/gstreamer/1.0/msvc_x86_64")

include_directories(${GSTREAMER_ROOT}/include
                    ${GSTREAMER_ROOT}/include/gstreamer-1.0
                    ${GSTREAMER_ROOT}/include/gstreamer-1.0/gst/video
                    ${GSTREAMER_ROOT}/include/gstreamer-1.0/gst/rtsp
                    ${GSTREAMER_ROOT}/include/glib-2.0
                    ${GSTREAMER_ROOT}/lib/glib-2.0/include)
 link_directories(${GSTREAMER_ROOT}/lib)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        player.h
        player.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GStreamerMSVC
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        handlergst.h handlergst.cpp
        screen.h screen.cpp
        playercontrolswidget.h playercontrolswidget.cpp
        bus_callback.h
        bus_callback.cpp
        ibuscallback.h ibuscallback.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET GStreamerMSVC APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(GStreamerMSVC SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(GStreamerMSVC
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(GStreamerMSVC PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    gstreamer-1.0.lib
    gobject-2.0.lib
    glib-2.0.lib
    gstvideo-1.0.lib
    gstrtsp-1.0.lib
)

set_target_properties(GStreamerMSVC PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS GStreamerMSVC
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GStreamerMSVC)
endif()
