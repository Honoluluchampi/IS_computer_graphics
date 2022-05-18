project(IS_computer_graphics)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-g3 -O0")

file(GLOB_RECURSE TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(app ${TARGETS})

target_include_directories(app PUBLIC 
  $ENV{HVE_DIR}/include 
  $ENV{HIE_DIR}/include 
  $ENV{HGE_DIR}/include
  ~/programs/external_libraries/imgui
  ~/programs/is_computer_graphics/basic1/include)

target_link_directories(app PUBLIC 
  $ENV{HNLL_ENGN}/lib
)

target_link_libraries(app PUBLIC 
  hge 
  hie 
  hve
  $ENV{VULKAN_DIR}/lib/libvulkan.so.1.3.204 
  $ENV{VULKAN_DIR}/lib/libvulkan.so.1 
  /usr/lib/x86_64-linux-gnu/libglfw.so.3.3
  X11
  Xtst
)