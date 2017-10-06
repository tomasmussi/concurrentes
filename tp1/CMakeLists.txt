cmake_minimum_required(VERSION 2.8.4)
project(tp1)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES main.cpp handlers/EventHandler.h handlers/MainSIGIntHandler.cpp handlers/MainSIGIntHandler.h handlers/NewPlayerHandler.h handlers/NewPlayerHandler.cpp handlers/SIGIntHandler.cpp handlers/SIGIntHandler.h ipc/Fifo.cpp ipc/Fifo.h ipc/FifoEscritura.cpp ipc/FifoEscritura.h ipc/FifoLectura.cpp ipc/FifoLectura.h ipc/LockFile.cpp ipc/LockFile.h ipc/MemoriaCompartida.h ipc/Semaphore.h ipc/Semaphore.cpp ipc/SignalHandler.cpp ipc/SignalHandler.h model/Match.cpp model/Match.h model/Person.cpp model/Person.h model/Team.cpp model/Team.h process/BeachManagerWorker.cpp process/BeachManagerWorker.h process/CourtManager.cpp process/CourtManager.h process/ResultsReporter.cpp process/ResultsReporter.h process/MatchProcess.cpp process/MatchProcess.h process/TeamMaker.cpp process/TeamMaker.h process/WorkerProcess.cpp process/WorkerProcess.h utils/Logger.cpp utils/Logger.h)


add_executable(tp1 ${SOURCE_FILES})
