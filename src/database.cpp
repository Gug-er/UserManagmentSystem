
#include "database.h"

Database::Database()
{
    std::string filepath = config.get("database.file");

    if(sqlite3_open(filepath.c_str(), &db) != SQLITE_OK)
    {
        LOG_ERROR(std::string("SQL: ") + sqlite3_errmsg(db));
    }

    sqlite3_exec
    (
        db,
        "CREATE TABLE IF NOT EXISTS users("
        "id INTEGER PRIMARY KEY,"
        "isAdmin INTEGER,"
        "login TEXT UNIQUE,"
        "password_hash TEXT)",
        NULL, NULL, NULL
    );

    LOG_DEBUG("DB: Database initialized");
}

Database::~Database()
{
    sqlite3_close(db);
    LOG_DEBUG("DB: users closed");
}

bool Database::check_login(std::string login) //Check if login is already taken
{
    LOG_DEBUG("DB: check_login called");
    sqlite3_stmt* stmt;
    const char* check_sql =
    "SELECT 1 FROM users WHERE login = ?"; 

    if(sqlite3_prepare_v2(db, check_sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        LOG_ERROR(std::string("SQL: ") + sqlite3_errmsg(db));
        return false;
    }
    sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
    {
        sqlite3_finalize(stmt);
        return false;
    }
    else
    {
        sqlite3_finalize(stmt);
        return true;
    }
    
}

std::string Database::get_hash(std::string login)
{
    LOG_DEBUG("DB: get_hash called");
    sqlite3_stmt* stmt;
    const char* check_sql =
    "SELECT password_hash FROM users WHERE login = ?";

    if(sqlite3_prepare_v2(db, check_sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        LOG_ERROR(std::string("SQL: ") + sqlite3_errmsg(db));
    }
    sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    std::string passwordHash;

    if(rc == SQLITE_ROW)
    {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        passwordHash = reinterpret_cast<const char*>(text);
    }
    else
    {
        LOG_AUTHF("User not found: " + login);
    }
    sqlite3_finalize(stmt);
    return passwordHash;
}

bool Database::register_user(std::string login, std::string password_hash)
{
    LOG_DEBUG("DB: register_user called");
    sqlite3_stmt* stmt;
    const char* insert_sql =
    "INSERT INTO users(isAdmin, login, password_hash) VALUES(0,?,?)";

    if(sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        LOG_ERROR(std::string("SQL: ") + sqlite3_errmsg(db));
    }
    sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password_hash.c_str(), -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) == SQLITE_DONE)
    {
        LOG_DEBUG("DB: User info added to db");
        sqlite3_finalize(stmt);
        return true;
    }
    else
    {
        LOG_ERROR(std::string("SQL: ") + sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }
}

std::vector<std::string> Database::list_users()
{
    LOG_DEBUG("DB: list_users called");
    sqlite3_stmt* stmt;
    const char* list_sql =
    "SELECT id, isAdmin, login FROM users";

    if(sqlite3_prepare_v2(db, list_sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        LOG_ERROR(std::string("SQL: ") + sqlite3_errmsg(db));
    }

    std::vector <std::string> users_list;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        int out = sqlite3_column_int(stmt, 1);
        const unsigned char* login = sqlite3_column_text(stmt, 2);

        std::string isAdmin;
        if(out == 1)
        {
            isAdmin = "*";
        }
        else
        {
            isAdmin = " ";
        }

        std::string row = "| " 
        + std::to_string(id) + " |   " 
        + isAdmin
        + std::string(reinterpret_cast<const char*>(login)) 
        + "    |";

        users_list.push_back(row);
    }
    sqlite3_finalize(stmt);
    return users_list;
}

bool Database::give_admin(std::string login)
{
    const char* admin_sql = 
    "UPDATE users SET isAdmin = 1 WHERE login = ?";

    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db, admin_sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            LOG_DEBUG("DB: Users listed");
            sqlite3_finalize(stmt);
            return true;
        }
        else
        {
            LOG_ERROR(std::string("SQL: ") + sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }
    }
    else
    {
        LOG_ERROR(std::string("SQL: ") + sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
}

bool Database::delete_user(std::string login)
{
    const char* delete_sql = 
    "DELETE FROM users WHERE login = ?";

    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db, delete_sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);

        if(sqlite3_step(stmt) == SQLITE_DONE)
        {
            LOG_DEBUG("DB: user " + login + "deleted");
            sqlite3_finalize(stmt);
            return true;
        }
        else
        {
            LOG_ERROR(std::string("SQL: ") + sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }
    }
    sqlite3_finalize(stmt);
    return false;
}

bool Database::check_status(std::string login)
{
    const char* status_sql =
    "SELECT isAdmin FROM users WHERE login = ?";

    sqlite3_stmt* stmt;
    int inter;
    if(sqlite3_prepare_v2(db, status_sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);

        sqlite3_step(stmt);

        inter = sqlite3_column_int(stmt, 0);
        LOG_DEBUG("DB: check_status completed");
    }
    else
    {
        LOG_ERROR(std::string("SQL: ") + sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return inter;
}