#include <iostream>
#include <string>

#include "logger.h"
#include "menu.h"

int main()
{
    LOG_INFO("MAIN: Application started");
    Menu menu;

    while (true)
    {
        menu.show_main_menu();
    }
    return 0;
}
