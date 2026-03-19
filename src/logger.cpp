#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <mutex>

#include "logger.h"

std::mutex mtx;

Logger::Logger()
{
    std::string authFile = config.get("logger.authfile");
    std::string file = config.get("logger.file");
    std::string logLevelstr = config.get("logger.level");

    authLogFile.open(authFile, std::ios::app);
    if (!authLogFile.is_open())
    {
        std::cerr << "Failed to open log file: " << authFile << std::endl;
        throw std::runtime_error("Failed to open authLog file");
    }

    logFile.open(file, std::ios::trunc);
    if (!logFile.is_open())
    {
        std::cerr << "Failed to open log file: " << file << std::endl;
        throw std::runtime_error("Failed to open log file");
    }
    currentLogLevel = parseLogLevel(logLevelstr);
}

void Logger::log(logLevel level, const std::string& msg)
{
    if (level < currentLogLevel) return;

    std::lock_guard<std::mutex> lock(mtx);

    std::string timeStr = getCurrentTime();
    std::string levelStr = levelToString(level);
    logFile << "[" << timeStr << "] [" << levelStr << "] " << msg << std::endl;
}

void Logger::logAuth(logLevel level, const std::string& msg)
{
    std::lock_guard<std::mutex> lock(mtx);

    std::string timeStr = getCurrentTime();
    std::string levelStr = levelToString(level);
    authLogFile << "[" << timeStr << "] [" << levelStr << "] " << msg << std::endl;
}

logLevel Logger::parseLogLevel(const std::string& levelStr)
{
    if (levelStr == "DEBUG") return logLevel::DEBUG;
    if (levelStr == "INFO") return logLevel::INFO;
    if (levelStr == "WARNING") return logLevel::WARNING;
    if (levelStr == "ERRORL") return logLevel::ERRORL;
    if (levelStr == "SUCCESS") return logLevel::SUCCESS;
    if (levelStr == "FAIL") return logLevel::FAIL;
    if (levelStr == "ADMIN") return logLevel::ADMIN;

    // Default to DEBUG if invalid
    return logLevel::DEBUG;
}

std::string Logger::levelToString(logLevel level)
{
    switch (level)
    {
        case logLevel::DEBUG: return "DEBUG";
        case logLevel::INFO: return "INFO";
        case logLevel::WARNING: return "WARNING";
        case logLevel::ERRORL: return "ERRORL";
        case logLevel::SUCCESS: return "SUCCESS";
        case logLevel::FAIL: return "FAIL";
        case logLevel::ADMIN: return "ADMIN";
        default: return "DEBUG";
    }
}

std::string Logger::getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_now = std::localtime(&now_time);

    std::ostringstream oss;
    oss << std::put_time(tm_now, "%Y-%m-%d %H:%M:%S");
    return oss.str();   
}

void Logger::debug(const std::string& msg)
{
    log(logLevel::DEBUG, msg);
}
void Logger::info(const std::string& msg)
{
    log(logLevel::INFO, msg);
}
void Logger::warning(const std::string& msg)
{
    log(logLevel::WARNING, msg);
}
void Logger::error(const std::string& msg)
{
    log(logLevel::ERRORL, msg);
}
void Logger::auths(const std::string msg)
{
    logAuth(logLevel::SUCCESS, "[AUTH] SUCCESS: " + msg);
}
void Logger::authf(const std::string msg)
{
    logAuth(logLevel::FAIL, "[AUTH] FAIL: " + msg);
}
void Logger::autha(const std::string msg)
{
    logAuth(logLevel::ADMIN, "[AUTH] ADMIN: " + msg);
}