#include <fstream>

#include "user_service.h"

User_service::User_service()
{

}

bool User_service::register_user(std::string login, std::string password)
{
    if(login == config.get("admin.username"))
    {
        return false;
    }
    if(database.check_login(login) == true)
    {
        std::string hashed_password = hash_password(password);
        if(database.register_user(login, hashed_password))
        {
            LOG_AUTHS(login);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool User_service::login_user(std::string login, std::string password)
{
    std::string admin_pass = get_admin_pass();
    if(login == config.get("admin.username"))
    {
        if(password == admin_pass)
        {
            LOG_AUTHA(login);
            return true;
        }
    }
    else if(hash_password(password) == database.get_hash(login))
    {
        LOG_AUTHS(login);
        return true;
    }
    else
    {
        LOG_AUTHF("Wrong password " + login);
        return false;
    }
    return false;
}

std::vector<std::string> User_service::list_users()
{
    return database.list_users();
}

std::string User_service::give_admin(std::string login)
{
    if(database.check_login(login) == false)
    {
        if(database.give_admin(login) == true)
        {
            return "Admin given";
        }
        else
        {
            return "Something went wrong";
        }
    }
    else
    {
        return "Login not found";
    }
}

std::string User_service::get_admin_pass()
{
    std::ifstream admin(config.get("admin.file"));

    if(!admin.is_open())
    {
        LOG_ERROR("US: Cannot open admin.txt");
    }
    std::string admin_pass;
    admin >> admin_pass;
    return admin_pass;
}

bool User_service::delete_user(std::string login)
{
    if(database.check_login(login) == false)
    {
        if(database.delete_user(login) == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool User_service::check_status(std::string login)
{
    return database.check_status(login);
}