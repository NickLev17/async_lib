#include <iostream>
#include "../include/logger.h"

#include "../include/Parser.h"
using namespace std;

Parser::Parser(size_t size) : size_pack(size)
{
    data.clear();
}

Parser::~Parser()
{
    if (log.joinable() || file1.joinable())
    {
        cout << "THREADS IS NOT COMPLETE !!!!!!!!!!!!!!\n";
    }

    if (log.joinable() || file2.joinable())
    {
        cout << "THREADS IS NOT COMPLETE !!!!!!!!!!!!!!\n";
    }
}

void Parser::showData()
{

    while (true)
    {
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [this]
                { return flag; });
        if (exit_log_flag)
            break;

        if (!data.empty())
        {
            cout << "bulk: ";
            for (const auto &d : data)
            {
                cout << d << " " << std::flush;
            }
            cout << "\n";
        }

        exit_log_flag = true;

        lk.unlock();
        cv.notify_all();
    }
}

void Parser::createLogFile1()
{

    while (true)
    {

        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [this]
                { return exit_log_flag; });
        if (exit_th1_flag)
            break;

        if (!data.empty())
        {

            if (filename1.empty())
            {
                std::ostringstream oss;
                oss << this;
                string temp = oss.str();

                filename1 = "file1_" + getCurrentTime() + " " + temp + ".txt";
            }
            Logger loger(filename1);

            if (data.size() % 2 != 0)
            {

                loger.log(data.at(0) + " thread 1");
                data.erase(data.begin());
            }
        }
        else
        {

            flag = false;
            exit_th1_flag = true;
        }

        lk.unlock();
        cv.notify_all();
    }
}

void Parser::createLogFile2()
{

    while (true)
    {

        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [this]
                { return exit_log_flag; });
        if (exit_th2_flag)
            break;

        if (!data.empty())
        {

            if (filename2.empty())
            {
                std::ostringstream oss;
                oss << this;
                string temp = oss.str();

                filename2 = "file2_" + getCurrentTime() + " " + temp + ".txt";
            }
            Logger loger(filename2);

            if (data.size() % 2 == 0)
            {

                loger.log(data.at(0) + " thread 2");
                data.erase(data.begin());
            }
        }
        else
        {

            flag = false;

            exit_th2_flag = true;
        }

        lk.unlock();
        cv.notify_all();
    }
}

void Parser::start(const char *buffer, size_t size)
{
    size_t pos = 0;
    data.clear();
    vectime.clear();

    std::function<void()> parseCommands = [&]()
    {
        while (pos < size)
        {

            while (pos < size && (buffer[pos] == ','))
                ++pos;
            if (pos >= size)
                return;

            if (buffer[pos] == ' ')
            {
                flag = true;

                cv.notify_all();
                return;
            }

            if (buffer[pos] == '{')
            {
                ++pos;
                parseCommands();
            }
            else if (buffer[pos] == '}')
            {
                ++pos;
                return;
            }
            else
            {

                size_t startPos = pos;
                while (pos < size && buffer[pos] != ' ' && buffer[pos] != '\n' && buffer[pos] != '\r' &&
                       buffer[pos] != '{' && buffer[pos] != '}' && buffer[pos] != ',')
                    ++pos;

                std::string cmd(buffer + startPos, pos - startPos);
                data.push_back(cmd);
                vectime.push_back(getCurrentTime());
            }
        }
    };

    parseCommands();

    flag = true;

    cv.notify_all();
}
string Parser::getCurrentTime()
{

    time_t now = time(0);

    tm *timeinfo = localtime(&now);
    char timestamp[20];

    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    return string(timestamp);
}
void Parser::startThreads()
{
    log = std::thread(&Parser::showData, this);
    file1 = std::thread(&Parser::createLogFile1, this);
    file2 = std::thread(&Parser::createLogFile2, this);
}
void Parser::joinThreads()
{
    if (log.joinable())
        log.join();
    if (file1.joinable())
        file1.join();
    if (file2.joinable())
        file2.join();
}
void Parser::restartFlags()
{
    flag = false;
    exit_log_flag = false;
    exit_th1_flag = false;
    exit_th2_flag = false;
}

void Parser::parsing(const char *data, size_t size)
{

    startThreads();
    start(data, size);
    joinThreads();
    restartFlags();
}
