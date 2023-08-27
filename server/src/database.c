#include <sqlite3.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>

#include "database.h"
#include "util.h"
#include "pwd_err.h"
#include "log.h"
#include "proto.h"


void init_db(struct sqlite3 *db)
{
    // clang-format off
    const char sql_users[] =
        "CREATE TABLE IF NOT EXISTS Users ( "
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username VARCHAR NOT NULL UNIQUE, "
        "password VARCHAR NOT NULL, "
        "token VARCHAR UNIQUE "
        ");";
    const char sql_passwords[] =
        "CREATE TABLE IF NOT EXISTS Passwords ( "
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "entry_name VARCHAR NOT NULL, "
        "username VARCHAR NOT NULL, "
        "password INTEGER NOT NULL, "
        "domains VARCHAR, "
        "description VARCHAR, "
        "user_id INTEGER, "
        "FOREIGN KEY(user_id) REFERENCES Users(id) "
        ");";
    // clang-format on

    if (sqlite3_exec(db, sql_users, 0, 0, 0) != SQLITE_OK)
    {
        log_err("DB", "Error in sqlite3_exec() using Users database\n");
        exit(-ENODATA);
    };
    if (sqlite3_exec(db, sql_passwords, 0, 0, 0) != SQLITE_OK)
    {
        log_err("DB", "Error in sqlite3_exec() using Passwords database\n");
        exit(-ENODATA);
    }
    return;
}


int32_t database_main(void)
{
    const char db_file[] = "passwords.db";


    struct sqlite3 *db = NULL;
    int res = 0;
    if (access(db_file, (R_OK | W_OK)) == 0)
    {
        res = sqlite3_open_v2(db_file, &db, DB_RW, NULL);
    }
    else
    {
        res = sqlite3_open_v2(db_file, &db, DB_RW_CREATE, NULL);
        if (chmod(db_file, DB_MODE))
        {
            log_err("DB", "Could not change DB permissions\n");
        }
    }
    if (res != SQLITE_OK)
    {
        log_err("DB", "Could not open database\n");
        exit(-ENODATA);
    }

    init_db(db);


    uint8_t msg_buf[MSG_ENC_SIZE_MAX];

    // TODO: Loop here

    sqlite3_close(db);
    return 0;
}