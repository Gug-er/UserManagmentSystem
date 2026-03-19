#include <string>
#include <vector>
#include <sqlite3.h>

#include "config.h"
#include "logger.h"

class Database
{
public:
    static Database& getInstance()
    {
        static Database instance;
        return instance;
    }

    bool give_admin(std::string login);
    bool register_user(std::string login, std::string hashed_password);
    bool delete_user(std::string login);
    
    bool check_login(std::string login);
    bool check_status(std::string login);

    std::vector<std::string> list_users();
    
    std::string get_hash(std::string login);
private:
    Config& config = Config::getInstance();
    Logger& logger = Logger::getInstance();

    sqlite3* db;

    Database();
    ~Database();
    Database(const Database&) = delete;
    Database& operator = (const Database&) = delete;
};