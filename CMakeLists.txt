cmake_minimum_required(VERSION 3.8)

project(robotic-lights)

add_executable(robotic-lights robotic-lights.c)
target_link_libraries(robotic-lights User32.lib Winmm.lib ucrt.lib msvcrt.lib)
set_target_properties(robotic-lights PROPERTIES LINK_FLAGS /SUBSYSTEM:Console)