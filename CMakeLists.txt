cmake_minimum_required(VERSION 3.12)
project(xfce-animated-wallpaper)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(build)
include(sanitizers)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0 IMPORTED_TARGET GLOBAL)
pkg_check_modules(GST REQUIRED gstreamer-1.0 IMPORTED_TARGET GLOBAL)
pkg_check_modules(GSTVIDEO REQUIRED gstreamer-video-1.0 IMPORTED_TARGET GLOBAL)

set(MODULE_NAME xfce-animated-wallpaper)

add_executable(${MODULE_NAME} src/main.cpp)

target_link_libraries(${MODULE_NAME}
    PRIVATE
        PkgConfig::GTK3
        PkgConfig::GST
        PkgConfig::GSTVIDEO
)
