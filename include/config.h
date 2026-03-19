#pragma once

#include <string>
#include <map>

class Config
{
public:

static Config& getInstance()
{
    static Config instance;
    return instance;
}

void readConfig(std::string path);


std::string get(std::string key);
//int getInt(std::string key);
//bool getBool(std::string key);

private:
    Config();
    Config(const Config&) = delete;
    Config& operator = (const Config&) = delete;

    std::map <std::string, std::string> configMap;
    //std::string getRequiredKeys(std::string key);
};