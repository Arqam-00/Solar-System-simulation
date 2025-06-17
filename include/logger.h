#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include "MyString.h"
#include<ctime>
using namespace std;

class Logger {
public:
    static Logger& getInstance() {
        static Logger obj;
        return obj;
    }

    void initialize(const String& filename_) {
        filename = filename_;
        file.open(filename_.C_Str(), ios::app);
        if (!file.is_open()) {
            cout << "file not found!\n";
        }
    }

    void cleanup() {
        file.close();
        file.open(filename.C_Str(), ios::trunc | ios::out);
        file.close();

    }

    void writeError(const String& error_message) {
        file <<"[Error]"<< error_message.C_Str();
        time_t now = time(0);
        char ttt[30];
        errno_t err = ctime_s(ttt, sizeof(ttt), &now);
        file << "  " << ttt;
    }
    void writeMessage(const String& error_message) {
        file << error_message.C_Str();
        time_t now = time(0);
        char ttt[30];
        errno_t err = ctime_s(ttt, sizeof(ttt), &now);
        file << "  " << ttt;
    }

private:
    Logger() {}
    ~Logger() {}
    fstream file;
    String filename;
};
#endif
