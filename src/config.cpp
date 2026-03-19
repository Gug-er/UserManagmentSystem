#include <iostream>
#include <fstream>
#include <string>

#include "config.h"

Config::Config()
{
    std::string path = "../config/config.ini";
    readConfig(path);
}

void Config::readConfig(std::string path)
{
    std::ifstream configFile(path);
    if (!configFile.is_open())
    {
        std::cout << path << "\n";
        std::cerr << "Failed to open config file." << std::endl;
        return;
    }

    std::string line, currentSection;

    while(std::getline(configFile, line))
    {
        if(line.find_last_not_of(" \t\n\r") != std::string::npos)
        {line.erase(line.find_last_not_of(" \t\n\r") + 1);};
        line.erase(0, line.find_first_not_of(" \t\n\r"));

        if(line.empty() || line[0] == ';' || line[0] == '#') {continue;};

        //Find section
        if(line.front() == '[' && line.back() == ']')
        {
            currentSection = line.substr(1, line.size() - 2);
            continue;
        }

        auto pos = line.find('=');
        if(pos == std::string::npos) continue;

        std::string variable, value;

        variable = line.substr(0, pos);
        value = line.substr(pos + 1);

        if(variable.find_last_not_of(" \t") != std::string::npos)
        {variable.erase(variable.find_last_not_of(" \t") + 1);};
        variable.erase(0, variable.find_first_not_of(" \t"));

        if(value.find_last_not_of(" \t") != std::string::npos)
        {value.erase(value.find_last_not_of(" \t") + 1);}
        value.erase(0, value.find_first_not_of(" \t"));

        std::string fullKey = currentSection.empty()
         ? variable 
         : currentSection + "." + variable;

        configMap[fullKey] = value;
    }
}

std::string Config::get (std::string key)
{
    std::string defaultValue = "";
    if(configMap.find(key) != configMap.end())
    {
    return configMap.at(key);
    }
    else
    {
        std::cout << key << " Not found\n";
        return defaultValue;
    }
}
//get required keys