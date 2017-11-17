cmake_minimum_required(VERSION 2.8.4)
project(tp2)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES main.cpp ipc/Cola.h Mensajes.h Servidor.cpp Servidor.h Cliente.cpp Cliente.h
        ipc/EventHandler.h ipc/SIGINT_Handler.h ipc/SignalHandler.h ipc/SignalHandler.cpp Servicio.cpp Servicio.h)
add_executable(tp2 ${SOURCE_FILES})