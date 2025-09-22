#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <cstring>
#include "cstddef"

using namespace std;

class Parser
{

    vector<string> vectime;
    vector<string> data;
    mutex mtx;
    mutex mtxfile;
    condition_variable cv;
    string filename1{};
    string filename2{};

    bool flag = false;
    bool exit_th1_flag = false;
    bool exit_th2_flag = false;
    bool exit_log_flag = false;

public:
    int size_pack{};
    explicit Parser(size_t size);

    ~Parser();

    void showData();

    void createLogFile1();

    void createLogFile2();

    void start(const char *buffer, size_t size);

    string getCurrentTime();

    void startThreads();

    void joinThreads();

    void restartFlags();

    void parsing(const char *data, size_t size);

private:
    std::thread log, file1, file2;
};

#endif