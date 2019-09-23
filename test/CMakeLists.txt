project(lazy-test)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE test.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE lazy catch2)