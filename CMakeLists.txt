project(CPP_UTILS)
cmake_minimum_required(VERSION 3.10.2)

file(GLOB UTILS_SRCS utils/*.cpp)
message("$UTILS_SRCS")

add_executable(example example_main.cpp ${UTILS_SRCS})

IF (WIN32)
    target_link_libraries(example psapi)
ENDIF()