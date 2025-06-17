#include <iostream>
#include <fstream>
#include "Mystring.h"
#include<ctime>
using namespace std;

class Logger {
public:
    static Logger& getInstance() {
        static Logger obj;
        return obj;
    }

    void initialize(const String& filename_) {
        if (!initialized) {
            filename = filename_;
            file.open(filename_.C_Str(), ios::app);
            if (!file.is_open()) {
                cout << "file not found!\n";
            }
            else {
                initialized = true;
            }
        }
    }

    void cleanup() {
        file.close();
        file.open(filename.C_Str(), ios::trunc | ios::out);
        file.close();

    }

    void writeError(const String& error_message) {
        file << " [Error]   "<< error_message.C_Str();
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
    bool initialized = false;
    Logger() {}
    ~Logger() {
        if (file.is_open()) {
            file.close();
        }
    }
    fstream file;
    String filename;
};

