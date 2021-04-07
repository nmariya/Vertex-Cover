cmake_minimum_required(VERSION 2.8)
project(ece650-prj)

if (EXISTS "${CMAKE_SOURCE_DIR}/minisat")
  add_subdirectory(minisat)
else()
  message (FATAL_ERROR "MiniSat not found. Use `git clone https://github.com/agurfinkel/minisat` to check it out.")
endif()


## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}-pthread")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/run)

# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/minisat)


# create the main executable
## add additional .cpp files if needed
add_executable(ece650-prj ece650-prj.cpp)
add_executable(ece650-a4 ece650-a4.cpp)
add_executable(ece650-a2 ece650-a2.cpp )
add_executable(ece650-a3 ece650-a3.cpp )
add_executable(rgen rgen.cpp)
add_executable(ece650-a2 ece650-a2.cpp )

target_link_libraries(simple ${CMAKE_THREAD_LIBS_INIT})

install(PROGRAMS ece650-a1.py DESTINATION bin)
install(TARGETS ece650-a2 ece650-a3 rgen RUNTIME DESTINATION bin)

# link MiniSAT libraries
target_link_libraries(ece650-prj minisat-lib-static)

# link MiniSAT libraries
#target_link_libraries(test minisat-lib-static)