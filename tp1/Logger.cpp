//
// Created by tomas on 29/09/17.
//

#include "Logger.h"

#include <sys/time.h>
#include <ctime>
#include <iomanip>
#include <sstream>

// Static
std::ofstream Logger::file_stream;
LockFile Logger::lock = LockFile("/dev/null");

std::string Logger::get_date() {
    char buffer[84];
    timeval current_time;
    gettimeofday(&current_time, NULL);
    int milli = current_time.tv_usec / 1000;
    char aux[80];
    strftime(aux, 80, "%Y-%m-%d %H:%M:%S", localtime(&current_time.tv_sec));
    sprintf(buffer, "%s.%d", aux, milli);
    return std::string(buffer);
}

void Logger::initialize_log() {
    file_stream << "BEGIN [" << get_date() << "]" << std::endl;
}

std::string Logger::get_error_flag(int error_level) {
    switch (error_level) {
        case ERROR:
            return "ERR";
        case WARNING:
            return "WAR";
        case DBG:
            return "DBUG";
        default:
            return "INFO";
    }
}

void Logger::log(const std::string& caller, int error, const std::string& error_message,
                 const std::string& timestamp) {
    lock.lock();
    file_stream << "[" << timestamp << "] [" << get_error_flag(error) << "] " << caller << ": " << error_message
            << std::endl;
    lock.release();
}

void Logger::open_logger(const std::string& log_file) {
    lock = LockFile("/tmp/logger_lock");
    file_stream.open(log_file.c_str(), std::ofstream::out | std::ofstream::app);
    lock.lock();
    initialize_log();
    lock.release();
}

void Logger::close_logger() {
    lock.lock();
    file_stream << "END [" << get_date()<< "]" << std::endl;
    file_stream.close();
    lock.release();
}