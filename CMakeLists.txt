cmake_minimum_required(VERSION 3.14)
project(wii-ui-latency-test LANGUAGES CXX)

include(FetchContent)

# imgui_gx2 accesses ImGui 1.91.0 internals, so keep these revisions paired.
FetchContent_Declare(
   imgui
   GIT_REPOSITORY https://github.com/ocornut/imgui.git
   GIT_TAG 8199457a7d9e453f8d3d9cadc14683fb54a858b5
   SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui)
FetchContent_Declare(
   imgui_gx2
   GIT_REPOSITORY https://github.com/Zenith-Team/imgui_gx2.git
   GIT_TAG cc0b51061074dd4cad2a79ca8a54eb770cfaa59a)
FetchContent_MakeAvailable(imgui imgui_gx2)

set(imgui_gx2_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include/imgui_gx2)
file(MAKE_DIRECTORY ${imgui_gx2_INCLUDE_DIR})
configure_file(${imgui_gx2_SOURCE_DIR}/imgui_impl_gx2.h
               ${imgui_gx2_INCLUDE_DIR}/imgui_impl_gx2.h COPYONLY)
configure_file(${imgui_gx2_SOURCE_DIR}/imgui_impl_wiiu.h
               ${imgui_gx2_INCLUDE_DIR}/imgui_impl_wiiu.h COPYONLY)

add_executable(wii-ui-latency-test
   source/main.cpp
   ${imgui_SOURCE_DIR}/imgui.cpp
   ${imgui_SOURCE_DIR}/imgui_draw.cpp
   ${imgui_SOURCE_DIR}/imgui_tables.cpp
   ${imgui_SOURCE_DIR}/imgui_widgets.cpp
   ${imgui_gx2_SOURCE_DIR}/imgui_impl_gx2.cpp
   ${imgui_gx2_SOURCE_DIR}/imgui_impl_wiiu.cpp)

target_include_directories(wii-ui-latency-test PRIVATE
   ${imgui_SOURCE_DIR}
   ${imgui_SOURCE_DIR}/..
   ${imgui_gx2_INCLUDE_DIR})
target_compile_definitions(wii-ui-latency-test PRIVATE
   IMGUI_DISABLE_DEFAULT_SHELL_FUNCTIONS)

set_target_properties(wii-ui-latency-test PROPERTIES OUTPUT_NAME drc-latency-test)

wut_create_rpx(wii-ui-latency-test)
