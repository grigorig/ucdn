cmake_minimum_required(VERSION 2.6)
project(ucdn)

add_library(ucdn_static STATIC ucdn.c ucdn.h)
add_library(ucdn_shared SHARED ucdn.c ucdn.h)
set_target_properties(ucdn_static PROPERTIES OUTPUT_NAME ucdn)
set_target_properties(ucdn_shared PROPERTIES OUTPUT_NAME ucdn)
set_target_properties(ucdn_shared PROPERTIES VERSION 1.0.0)
set_target_properties(ucdn_shared PROPERTIES SOVERSION 1)

install(TARGETS ucdn_static ucdn_shared
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
install(FILES ucdn.h DESTINATION include)
