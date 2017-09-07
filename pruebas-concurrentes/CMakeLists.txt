cmake_minimum_required(VERSION 2.8.4)
project(pruebas_concurrentes)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp shared-memory/SHMUsage.cpp shared-memory/SHMUsage.h
        shared-memory/MemoriaCompartida.h shared-memory/MemoriaCompartida.h)
add_executable(pruebas_concurrentes ${SOURCE_FILES})