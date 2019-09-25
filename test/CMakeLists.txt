project(lazy-test)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
conan_define_targets()

add_executable(${PROJECT_NAME} test.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE lazy)