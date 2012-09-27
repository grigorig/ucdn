cmake_minimum_required(VERSION 2.6)
project(ucdn C)

ADD_DEFINITIONS(-O2 -Wall -std=c89)

add_library(ucdn_static STATIC ucdn.c ucdn.h)
add_library(ucdn_shared SHARED ucdn.c ucdn.h)
set_target_properties(ucdn_static PROPERTIES OUTPUT_NAME ucdn)
set_target_properties(ucdn_shared PROPERTIES OUTPUT_NAME ucdn)
set_target_properties(ucdn_shared PROPERTIES VERSION 1.0.0)
set_target_properties(ucdn_shared PROPERTIES SOVERSION 1)

add_executable(ucdn-test ucdn-test.c)
target_link_libraries(ucdn-test ucdn_static)

install(TARGETS ucdn_static ucdn_shared
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
install(FILES ucdn.h DESTINATION include)
