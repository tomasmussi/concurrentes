#ifndef TP1_LOGGER_H
#define TP1_LOGGER_H

#include <iostream>
#include <string>
#include <fstream>
#include "../ipc/LockFile.h"

class Logger {
private:
    static std::ofstream file_stream;
    static LockFile lock;

    static std::string get_error_flag(int error_type);
    static void initialize_log();

public:
    static const int DEBUG = 0;
    static const int INFO = 1;
    static const int ERROR = 2;
    static const int WARNING = 3;

    static void open_logger(const std::string& log_file);
    static void log(const std::string& caller, int error, const std::string& error_message,
                    const std::string& timestamp);
    static void close_logger();
    static std::string get_date();
};

#endif //TP1_LOGGER_H
