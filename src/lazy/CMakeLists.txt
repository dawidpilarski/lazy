project(lazy VERSION 0.1.0)

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE lazy.hpp)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_include_directories(${PROJECT_NAME} INTERFACE "${PARENT_DIR}")
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)