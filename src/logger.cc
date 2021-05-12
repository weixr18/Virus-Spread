#include "logger.h"

Logger::Logger()
{
}

Logger::~Logger()
{
}

bool Logger::Start(const std::string &file_name)
{
    outfile.open(file_name, std::ios::app);
    if (!outfile.is_open())
    {
        std::cout << "Open file failure" << std::endl;
        return false;
    }
    return true;
}

void Logger::Log(const std::string &s)
{
    outfile << s;
}

void Logger::Close()
{
    outfile.close();
}