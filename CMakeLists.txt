cmake_minimum_required(VERSION 3.15)
project(SignalCompiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(SignalCompiler ${SOURCES})

add_custom_command(TARGET SignalCompiler POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/tests"
        "$<TARGET_FILE_DIR:SignalCompiler>"
)