cmake_minimum_required(VERSION 3.17)
project(WebClient)

set(CMAKE_CXX_STANDARD 14)

add_executable(WebServer main.cpp
        InputProcessing.cpp
        InputProcessing.hpp
        SingleThreadPool.hpp
        SingleThreadPool.cpp
        Worker.cpp
        Worker.hpp
        GlobalVar.hpp
        InvalidInputFormat.cpp
        InvalidInputFormat.hpp
        )