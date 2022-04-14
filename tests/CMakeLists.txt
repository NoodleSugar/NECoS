cmake_minimum_required(VERSION 3.22.0)
project(
    NECoSTests
        LANGUAGES CXX
)

add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/NecosTests.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        NECoS
)

add_test(NAME ${PROJECT_NAME} COMMAND AllTests)