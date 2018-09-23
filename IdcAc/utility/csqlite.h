#ifndef CSQLITE_H
#define CSQLITE_H
#include "include_all_qt.h"
#include <sqlite3.h>
class CSqlite
{
public:
    CSqlite();
    ~CSqlite();

public:
    int sql_open();
    int sql_close();
    int sql_exe(const char *sql_cmd,
                         int (*call_back)(void* cb_param, int n_count, char** pp_value, char** pp_name) = NULL,
                         void* call_back_param = NULL);

private:
    const char* sql_err_msg();
    void sql_free(void* err_msg);

public:
    int sql_test();

    void SetSqlName(const char* sql_name){strSqlName = sql_name;}
    void SetTableName(const char* table_name){strTableName = table_name;}

private:
    sqlite3* pSqlDb;
    string strSqlName;
    string strTableName;
    char* pErr;
};

#endif // CSQLITE_H
