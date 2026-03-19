#include <string>
#include <vector>

#include "hash.h"
#include "database.h"
#include "logger.h"
#include "config.h"

class User_service
{
public:
    static User_service& getInstance()
    {
        static User_service instance;
        return instance;
    }

    bool login_user(std::string login, std::string password);
    std::vector<std::string> list_users();
    bool register_user(std::string login, std::string password);
    bool delete_user(std::string login);
    std::string give_admin(std::string login);
    bool check_status(std::string login);
private:
    Database& database = Database::getInstance();
    Config& config = Config::getInstance();

    std::string get_admin_pass();

    User_service();
    User_service(const User_service&) = delete;
    User_service& operator = (const User_service&) = delete;
};