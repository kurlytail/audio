add_subdirectory(wxWidgets)
add_subdirectory(yaml-cpp)
add_subdirectory(osx)
add_subdirectory(rtmidi)
add_subdirectory(portaudio)

find_package (glog REQUIRED)
find_package (PkgConfig REQUIRED)
find_package (gflags REQUIRED)
find_package (folly REQUIRED)
find_package (fmt REQUIRED)

pkg_check_modules (PORTAUDIO REQUIRED portaudio-2.0)

add_library(glog INTERFACE)
target_link_libraries(glog INTERFACE glog::glog)
target_include_directories(glog INTERFACE glog::glog)

add_library(folly INTERFACE)
target_link_libraries(folly INTERFACE Folly::folly)
target_include_directories(folly INTERFACE Folly::folly)

add_library(fmt INTERFACE)
target_link_libraries(fmt INTERFACE fmt::fmt)
target_include_directories(fmt INTERFACE fmt::fmt)