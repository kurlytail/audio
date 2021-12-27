find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_library(wxWidgets INTERFACE)
target_link_directories(wxWidgets INTERFACE {wxWidgets_LIBRARIES})
target_include_directories(wxWidgets INTERFACE ${wxWidgets_INCLUDE_DIRS})
