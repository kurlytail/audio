
ExternalProject_Add(
    rtmidi-ext
    GIT_REPOSITORY https://github.com/thestk/rtmidi.git
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
)

add_library(rtmidi INTERFACE)
add_dependencies(rtmidi rtmidi-ext)
target_link_directories(rtmidi INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/lib)
target_include_directories(rtmidi INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_libraries(rtmidi INTERFACE -lrtmidi)