#include <string>

#include "user_service.h"
#include "logger.h"
#include "config.h"

class Menu
{
public:
    Menu();
    void show_main_menu();

    private:
        void login_user();
        void register_user();
        void list_users();

    void show_user_menu();

    void show_admin_menu();
    void manage_users();
        void delete_user();
        void give_admin();

    
    std::string get_password();
    std::string get_login();
private:
    Config& config = Config::getInstance();
    User_service& user_service = User_service::getInstance();
};