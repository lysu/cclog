cmake_minimum_required(VERSION 2.8)
project(cclog)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -g")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(GLIB2)

set(SOURCE_FILES main.c cclog.c cclog.h ccbuffer.c ccbuffer.h cclogfile.c cclogfile.h)
add_executable(cclog ${SOURCE_FILES})

include_directories(${GLIB2_INCLUDE_DIRS})
target_link_libraries(cclog pthread ${GLIB2_LIBRARIES})


