#include "TableReporter.h"
#include "Person.h"
#include "Logger.h"

TableReporter::TableReporter()
   : _shared_memory("/bin/bash", 'a'), _time(0){ //CAMBIAR PATH
}

TableReporter::~TableReporter() {
}

int TableReporter::do_work() {
    sleep(_time);
    return 0;
}

void TableReporter::initialize() {
}

void TableReporter::finalize() {
}

std::string TableReporter::prettyName() {
    return "Table Reporter";
}
