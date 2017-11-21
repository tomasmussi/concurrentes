cmake_minimum_required(VERSION 2.8.4)
project(tp2)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES main.cpp Mensajes.h Servidor.cpp Servidor.h Cliente.cpp Cliente.h Servicio.cpp Servicio.h Constantes.h
        ipc/EventHandler.h ipc/SIGINT_Handler.h ipc/SignalHandler.h ipc/SignalHandler.cpp ipc/Cola.h ServicioMonedas.h ServicioMonedas.cpp ServicioTiempo.cpp ServicioTiempo.h)
add_executable(tp2 ${SOURCE_FILES})