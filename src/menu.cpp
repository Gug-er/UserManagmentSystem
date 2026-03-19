#include <iostream>
#include <vector>

#include <windows.h>
#include <conio.h>

#include "menu.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

Menu::Menu()
{
    SetConsoleTextAttribute(hConsole, 3); //Cyan
    std::cout <<
    "======================\n"
    "User Managment System\n"
    "=======================\n";
    LOG_DEBUG("Menu initialized");
}

void Menu::show_main_menu()
{
    SetConsoleTextAttribute(hConsole, 1); //Blue
    std::cout <<
    "=======================\n"
    "       MAIN MENU\n"
    "======================\n"
    "1. Login\n"
    "2. Register\n"
    "3. Exit\n";

    int input;
    std::cin >> input;

    switch(input)
    {
        case 1:
        {
            login_user();
            break;
        }
        case 2:
        {
            register_user();
            break;
        }
        case 3:
        {
            exit(0);
        }
        default:
        {
        std::cout << "\033[2J\033[1;1H";
        SetConsoleTextAttribute(hConsole, 4); //Red
            std::cout << "Invalid input";

            std::cout << "\nPress enter to continue...";
            std::cin.ignore();
            std::cin.get();

            break;
        }
    }
}

void Menu::show_user_menu()
{
    SetConsoleTextAttribute(hConsole, 5); //Pink
    std::cout <<
    "=======================\n"
    "       USER MENU\n"
    "======================\n"
    "1. List users\n"
    "2. To main menu\n"
    "3. Exit\n";

    int input;
    std::cin >> input;

    switch(input)
    {
        case 1:
        {
            list_users();
            break;
        }
        case 2:
        {
            show_main_menu();
            break;
        }
        case 3:
        {
            exit(0);
        }
        default:
        {
            std::cout << "\033[2J\033[1;1H";
            SetConsoleTextAttribute(hConsole, 4); //Red
            std::cout << "Invalid input";
            std::cout << "\nPress enter to continue...";
            std::cin.ignore();
            std::cin.get();
            show_user_menu();
            break;
        }
    }
}

void Menu::show_admin_menu()
{
    SetConsoleTextAttribute(hConsole, 5); //Pink
    std::cout <<
    "=======================\n"
    "       ADMIN MENU\n"
    "======================\n"
    "1. Manage users\n"
    "2. To main menu\n"
    "3. List users\n"
    "4. Exit\n";

    int input;
    std::cin >> input;

    switch(input)
    {
        case 1:
        {
            manage_users();
            break;
        }
        case 2:
        {
            show_main_menu();
            break;
        }
        case 3:
        {
            list_users();
            break;
        }
        case 4:
        {
            exit(0);
        }
        default:
        {
            std::cout << "\033[2J\033[1;1H";
            SetConsoleTextAttribute(hConsole, 4); //Red
            std::cout << "Invalid input";
            std::cout << "\nPress enter to continue...";
            std::cin.ignore();
            std::cin.get();
            show_admin_menu();
            break;
        }
    }
}

void Menu::manage_users()
{
    SetConsoleTextAttribute(hConsole, 5); //Pink
    user_service.list_users();
    std::cout <<
    "1. Delete user\n"
    "2. Give admin\n"
    "3. Back to admin menu\n";

    int input;
    std::cin >> input;

    switch(input)
    {
        case 1:
        {
            delete_user();
            break;
        }
        case 2:
        {
            give_admin();
            break;
        }
        case 3:
        {
            show_admin_menu();
            break;
        }
        default:
        {
            std::cout << "\033[2J\033[1;1H";
            SetConsoleTextAttribute(hConsole, 4); //Red
            std::cout << "Invalid input";
            std::cout << "\nPress enter to continue...";
            std::cin.ignore();
            std::cin.get();
            manage_users();
            break;
        }
    }
}

void Menu::login_user()
{
    std::cout <<
    "=======================\n"
    "         LOGIN\n"
    "=======================\n";

    std::cout << "login: ";
    std:: string login = get_login();

    std::cout << "password: ";
    std::string password = get_password();


    if(user_service.login_user(login, password) == true 
    && login == config.get("admin.username"))
    {
        show_admin_menu();
    }
    else if(user_service.login_user(login, password) == true 
    && user_service.check_status(login) == true)
    {
        show_admin_menu();
    }
    else if(user_service.login_user(login, password) == true)
    {
        show_user_menu();
    }
    else
    {
        std::cout << "\033[2J\033[1;1H";
        SetConsoleTextAttribute(hConsole, 4); //Red
        std::cout << "Incorrect login or password\n";
        std::cout << "\nPress enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }
}

void Menu::register_user()
{
    SetConsoleTextAttribute(hConsole, 1); //Blue
    std::cout << "\033[2J\033[1;1H"; //Clear console
    std::cout <<
    "=======================\n"
    "      REGISTRATION\n"
    "=======================\n";

    std::cout << "login: ";
    std:: string login = get_login();

    std::cout << "password: ";
    std:: string password = get_password();

    std::cout << login + " " + password << std::endl;
    bool success = user_service.register_user(login, password);
    if(success)
    {
        std::cout << "\033[2J\033[1;1H";
        std::cout <<
        "Registration complete!\n";
        //To user menu
    }
    else
    {
        std::cout << "\033[2J\033[1;1H";
        SetConsoleTextAttribute(hConsole, 4); //Red
        std::cout <<
        "\nLogin taken";

        std::cout << "\nPress enter to continue...";
        std::cin.ignore();
        std::cin.get();

        register_user();
    }
}

void Menu::list_users()
{
    SetConsoleTextAttribute(hConsole, 1); //Blue
    std::cout << "\033[2J\033[1;1H"; //Clear console
    std::cout <<
    "=======================\n"
    "   REGISTERED USERS\n"
    "=======================\n"
    "| ID |    Login    |\n"
    "___________________\n";
    
    std::vector<std::string> users_list = user_service.list_users();

    for(const auto& row : users_list)
    {
        std::cout << row << std::endl;
    }

    std::cout << "\nPress enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

void Menu::give_admin()
{
    std::cout << "login: ";
    std::string login = get_login();
    std::string result = user_service.give_admin(login);

    if(result == "Admin given")
    {
        std::cout 
        << "\nAdmin given to user: " 
        << login << std::endl;
    }
    else if(result == "Login not found")
    {
        std::cout
        << "\nLogin " << login
        << " not found\n";
    }
    else
    {
        std::cout << "\nSomething went wrong. Try again later\n";
    }
    manage_users();
}

void Menu::delete_user()
{
    std::cout << "\nlogin: ";
    std::string login = get_login();

    if(user_service.delete_user(login) == true)
    {
        std::cout << "\nUser deleted\n";
    }
    else
    {
        std::cout << "\nSomething went wrong. Try again later\n";
    }
    manage_users();
}

std::string Menu::get_password()
{
    const size_t MAX_LENGTH = 16; //Max length of input
    std::string password;
    char ch;

    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b') //Backspace
        {
            if (!password.empty())
            {
                password.pop_back();
                std::cout << "\b \b"; // backspace
            }
        }
        else if (password.size() < MAX_LENGTH)
        {
            password += ch;
            std::cout << ch;
        }
        else
        {
            std::cout << "\a";
        }
    }

    std::cout << std::endl;
    return password;
}

std::string Menu::get_login()
{
    const size_t MAX_LENGTH = 16; //Max length of input
    std::string login;
    char ch;

    while ((ch = _getch()) != '\r')
    {
        if (login.size() < MAX_LENGTH)
        {
            login += ch;
            std::cout << ch;
        }
        else
        {
            std::cout << "\a";
        }
    }
    std::cout << std::endl;
    return login;
}