cmake_minimum_required(VERSION 2.8.4)
project(tp1)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES main.cpp Match.cpp Match.h MatchesRecorder.cpp MatchesRecorder.h MatchPlayed.cpp MatchPlayed.h WorkerProcess.cpp WorkerProcess.h PeopleRegisterWorker.cpp PeopleRegisterWorker.h BeachManagerWorker.cpp BeachManagerWorker.h SignalHandler.cpp SignalHandler.h EventHandler.h SIGIntHandler.cpp SIGIntHandler.h MainSIGIntHandler.cpp MainSIGIntHandler.h MemoriaCompartida.h Fifo.cpp Fifo.h FifoEscritura.cpp FifoEscritura.h FifoLectura.cpp FifoLectura.h TeamMaker.cpp TeamMaker.h Person.cpp Person.h Logger.cpp Logger.h LockFile.cpp LockFile.h Team.cpp Team.h CourtManager.cpp CourtManager.h CourtManager.cpp CourtManager.h TableReporter.h TableReporter.cpp ResultsReporter.h ResultsReporter.cpp)
add_executable(tp1 ${SOURCE_FILES})
