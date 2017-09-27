cmake_minimum_required(VERSION 2.8.4)
project(tp1)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES main.cpp Subscriber.cpp Subscriber.h TournamentManager.cpp TournamentManager.h Match.cpp Match.h MatchesRecorder.cpp MatchesRecorder.h MatchPlayed.cpp MatchPlayed.h WorkerProcess.cpp WorkerProcess.h PeopleRegisterWorker.cpp PeopleRegisterWorker.h BeachManagerWorker.cpp BeachManagerWorker.h SignalHandler.cpp SignalHandler.h EventHandler.h SIGIntHandler.cpp SIGIntHandler.h MainSIGIntHandler.cpp MainSIGIntHandler.h MemoriaCompartida.h)
add_executable(tp1 ${SOURCE_FILES})