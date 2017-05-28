cmake_minimum_required(VERSION 2.6)
project(ucdn C CXX)
include(CMakeToolsHelpers OPTIONAL)

add_definitions(-O3 -Wall -Werror -pedantic)

add_library(ucdn ucdn.cpp ucdn.h)
#set_target_properties(ucdn PROPERTIES LINKER_LANGUAGE C)
set_property(TARGET ucdn PROPERTY CXX_STANDARD 14)

if(BUILD_SHARED_LIBS)
    set_target_properties(ucdn PROPERTIES VERSION 1.0.0)
    set_target_properties(ucdn PROPERTIES SOVERSION 1)
endif()

add_executable(ucdn-test ucdn-test.c)
target_link_libraries(ucdn-test ucdn)
set_target_properties(ucdn-test PROPERTIES LINKER_LANGUAGE C)

install(TARGETS ucdn
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
install(FILES ucdn.h DESTINATION include)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Check REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CHECK_INCLUDE_DIRS})

add_executable(ucdn-unit-test ucdn-unit-test.c)
target_link_libraries(ucdn-unit-test ucdn ${CHECK_LIBRARIES} pthread)
set_target_properties(ucdn-unit-test PROPERTIES LINKER_LANGUAGE C)

enable_testing()
add_test(NAME ucdn-unitttest COMMAND ucdn-unit-test)
