cmake_minimum_required (VERSION 3.0.2)
include(ExternalProject)

project (device VERSION 1.0)

add_subdirectory(external)
add_executable(device main.cpp)

target_link_libraries(device PRIVATE
    osx
    wxWidgets
    glog
    folly
    fmt
    portaudio
    yaml-cpp
    rtmidi
)
