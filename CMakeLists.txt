cmake_minimum_required(VERSION 3.2)
project(wii-ui-latency-test LANGUAGES CXX)

add_executable(wii-ui-latency-test source/main.cpp)

wut_create_rpx(wii-ui-latency-test)
