cmake_minimum_required(VERSION 3.15)
project(StateMachine)

set(CMAKE_CXX_STANDARD 17)

add_executable(StateMachine main.cpp "StateMachine.hpp" "StateHolder.hpp" "StateHolder.cpp")