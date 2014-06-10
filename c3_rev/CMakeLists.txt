project(c3_rev)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
file(GLOB INC_LIST "*.h")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -g")

add_executable(${PROJECT_NAME} ${INC_LIST} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} "winmm.lib")
target_link_libraries(${PROJECT_NAME} "ddraw.lib")
target_link_libraries(${PROJECT_NAME} "dsound.lib")

