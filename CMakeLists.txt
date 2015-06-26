cmake_minimum_required(VERSION 3.2)
project(GameBoyEmulator)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

#set(SOURCE_FILES main.cpp)
add_executable(GameBoyEmulator ${SOURCE_FILES}  src/main.cpp
                                                src/Processor.cpp
                                                src/GameBoy.hpp
                                                src/GameBoy.cpp src/MemoryManagementUnit.hpp src/MemoryManagementUnit.cpp src/Display.hpp src/Display.cpp src/Timer.hpp src/Timer.cpp src/Input.hpp src/Input.cpp)

target_link_libraries(GameBoyEmulator ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsfml-graphics.a
                                      ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsfml-window.a
                                      ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsfml-audio.a
                                      ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsfml-network.a
                                      ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsfml-system.a)