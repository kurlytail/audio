
ExternalProject_Add(
    yaml-cpp-ext
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
)

add_library(yaml-cpp INTERFACE)
add_dependencies(yaml-cpp yaml-cpp-ext)
target_link_directories(yaml-cpp INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/lib)
target_include_directories(yaml-cpp INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_libraries(yaml-cpp INTERFACE -lyaml-cpp)