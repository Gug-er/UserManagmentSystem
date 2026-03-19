#pragma once

#include <string>
#include <fstream>
#include "config.h"

//Macro
#define LOG_DEBUG(msg) Logger::getInstance().debug(msg)
#define LOG_INFO(msg)  Logger::getInstance().info(msg)
#define LOG_WARNING(msg) Logger::getInstance().warning(msg)
#define LOG_ERROR(msg) Logger::getInstance().error(msg)
#define LOG_AUTHS(msg) Logger::getInstance().auths(msg)
#define LOG_AUTHF(msg) Logger::getInstance().authf(msg)
#define LOG_AUTHA(msg) Logger::getInstance().autha(msg)

enum class logLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERRORL,
    SUCCESS,
    FAIL,
    ADMIN
};

class Logger
{
public:
    static Logger& getInstance()
    {
        static Logger instance;
        return instance;
    }

    void log(logLevel level, const std::string& msg);
    void logAuth(logLevel level, const std::string& msg);

    void debug(const std::string& msg);
    void info(const std::string& msg);
    void warning(const std::string& msg);
    void error(const std::string& msg);
    void auths(const std::string msg);
    void authf(const std::string msg);
    void autha(const std::string msg);

private:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator = (const Logger&) = delete;

    logLevel currentLogLevel;
    std::ofstream logFile;
    std::ofstream authLogFile;

    std::string levelToString(logLevel level);
    std::string getCurrentTime();

    logLevel parseLogLevel(const std::string& levelStr);

    Config& config = Config::getInstance();
};