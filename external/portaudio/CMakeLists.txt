
ExternalProject_Add(
    portaudio-ext
    GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
)

add_library(portaudio INTERFACE)
add_dependencies(portaudio portaudio-ext)
target_link_directories(portaudio INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/lib)
target_include_directories(portaudio INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_libraries(portaudio INTERFACE -lportaudio)