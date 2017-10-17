cmake_minimum_required(VERSION 2.8)
project(04.Pipes_y_fifos)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DEJEMPLO_1")
add_executable(ejemplo1 main1.cc Pipes/Pipe.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DEJEMPLO_2")
add_executable(ejemplo2 main2.cc Pipes/Pipe.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DEJEMPLO_3")
add_executable(ejemplo3 main3.cc Fifos/Fifo.cpp Fifos/FifoEscritura.cpp Fifos/FifoLectura.cpp)
