cmake_minimum_required(VERSION 2.8.4)
project(tp1)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES main.cpp MatchProcess.cpp MatchProcess.h WorkerProcess.cpp WorkerProcess.h BeachManagerWorker.cpp BeachManagerWorker.h SignalHandler.cpp SignalHandler.h EventHandler.h SIGIntHandler.cpp SIGIntHandler.h MainSIGIntHandler.cpp MainSIGIntHandler.h MemoriaCompartida.h Fifo.cpp Fifo.h FifoEscritura.cpp FifoEscritura.h FifoLectura.cpp FifoLectura.h TeamMaker.cpp TeamMaker.h Person.cpp Person.h Logger.cpp Logger.h LockFile.cpp LockFile.h Team.cpp Team.h CourtManager.cpp CourtManager.h CourtManager.cpp CourtManager.h NewPlayerHandler.h NewPlayerHandler.cpp Match.cpp Match.h)
add_executable(tp1 ${SOURCE_FILES})