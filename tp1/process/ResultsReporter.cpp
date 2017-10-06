#include "ResultsReporter.h"
#include "../ipc/SignalHandler.h"

ResultsReporter::ResultsReporter(){
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
