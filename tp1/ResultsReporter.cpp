#include "ResultsReporter.h"
#include "Person.h"
#include "Logger.h"

ResultsReporter::ResultsReporter()
   : _shared_memory("/bin/bash", 'a'){ //CAMBIAR PATH
}

ResultsReporter::~ResultsReporter() {
}

int ResultsReporter::do_work() {
    return 0;
}

void ResultsReporter::initialize() {
}

void ResultsReporter::finalize() {
}

std::string ResultsReporter::prettyName() {
    return "Results Reporter";
}
