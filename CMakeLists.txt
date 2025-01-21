cmake_minimum_required(VERSION 3.24)
project(Ticket-system)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O2")

include_directories(
        ${PROJECT_SOURCE_DIR}/lib/include ${PROJECT_SOURCE_DIR}/lib/template
        ${PROJECT_SOURCE_DIR}/backend/include ${PROJECT_SOURCE_DIR}/backend/template)
file(GLOB src_lib ${PROJECT_SOURCE_DIR}/lib/src/*.cpp)
file(GLOB src_backend ${PROJECT_SOURCE_DIR}/backend/src/*.cpp)
set(src_list ${src_lib} ${src_backend})

add_executable(code ${src_list} main.cpp)