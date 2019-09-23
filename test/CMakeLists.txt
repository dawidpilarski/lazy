project(lazy-test)

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE test.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE lazy)