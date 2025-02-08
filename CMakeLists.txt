cmake_minimum_required(VERSION 3.24)
project(Ticket-system)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

include_directories("${PROJECT_SOURCE_DIR}/backend/include")

add_subdirectory(
    lib
    backend
)

add_executable(code main.cpp)

target_link_libraries(code PRIVATE ${lib_targets} ${backend_targets})