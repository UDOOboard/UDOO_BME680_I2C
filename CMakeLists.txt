cmake_minimum_required(VERSION 2.8)
project(UDOO_BME680_I2C)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp lib)
add_executable(UDOO_BME680_I2C ${SOURCE_FILES})