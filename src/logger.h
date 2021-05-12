#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

class Logger
{
public:
    Logger();
    ~Logger();
    bool Start(const std::string &file_name);
    void Log(const std::string &s);
    void Close();

private:
    std::ofstream outfile;
};

#endif //_LOGGER_H_
