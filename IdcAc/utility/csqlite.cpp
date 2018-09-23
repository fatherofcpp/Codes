#include "csqlite.h"
#include "Log.h"
#define TRACE qDebug

CSqlite::CSqlite()
{
    pSqlDb = NULL;
    strSqlName = "test.db";
    strTableName = "tb_test";
    pErr = new char[32];
    strcpy(pErr, "catch err fail");
}

CSqlite::~CSqlite()
{
    delete pErr;
    sql_close();
}

int callback_select(void* param_cb ,int n_count,char** pp_value,char** pp_name)
{
    param_cb = param_cb;
    string s;
    for(int i = 0; i < n_count; i++)
    {
        s += pp_name[i];
        s += ":";
        s += pp_value[i];
        s += ";";
    }
//    TRACE("[%s][%s]\n", __FUNCTION__, s.c_str());
    return 0;
}

int CSqlite::sql_open()
{
    if(pSqlDb != NULL)
    {
        gLog.Add("[%s] [%s] already opened", __FUNCTION__, strSqlName.c_str());
        return 0;
    }

    int ret = 0;

    try
    {
        ret = sqlite3_open(strSqlName.c_str(), &pSqlDb);  //打开（或新建）一个数据库
    }
    catch(exception &e)
    {
        gLog.Add("[%s] sqlite3_open err catch[%s]", __FUNCTION__, e.what());
        pSqlDb = NULL;
        ret = -1;
    }
    catch(...)
    {
        gLog.Add("[%s] sqlite3_open err catch", __FUNCTION__);
        pSqlDb = NULL;
        ret = -1;
    }

    if(ret == SQLITE_OK)
    {
        //TRACE("[%s] sqlite3_open[%s] suc", __FUNCTION__, strSqlName.c_str());
        return 0;
    }
    else
    {
        gLog.Add("[%s] sqlite3_open[%s] fail err:[%s]",
              __FUNCTION__, strSqlName.c_str(), sql_err_msg());
        return -1;
    }
}

int CSqlite::sql_close()
{
    if(pSqlDb == NULL)
    {
        gLog.Add("[%s] already closed", __FUNCTION__);
        return 0;
    }

    int ret = 0;
    try
    {
        ret = sqlite3_close(pSqlDb); //关闭数据库
        pSqlDb = NULL;
    }
    catch(exception &e)
    {
        gLog.Add("[%s] sqlite3_close err catch[%s]", __FUNCTION__, e.what());
        ret = -1;
    }
    catch(...)
    {
        gLog.Add("[%s] sqlite3_close err catch", __FUNCTION__);
        ret = -1;
    }

    if(ret != SQLITE_OK)
    {
        gLog.Add("[%s] sqlite3_close fail err:[%s]",
              __FUNCTION__, sql_err_msg());
        return -1;
    }
    else
    {
        return 0;
    }
}

int CSqlite::sql_exe(const char *sql_cmd,
                     int (*call_back)(void*,int,char**,char**),
                     void* call_back_param)
{
    if(pSqlDb == NULL)
    {
        gLog.Add("[%s] sqlite3_exec[%s] fail, pSqlDb == NULL ",
              __FUNCTION__, sql_cmd);
        return -1;
    }

    char* err_msg = NULL;
    int ret = 0;
    try
    {
        ret = sqlite3_exec(pSqlDb, sql_cmd, call_back, call_back_param, &err_msg);
    }
    catch(exception &e)
    {
        gLog.Add("[%s] sqlite3_exec err catch[%s]", __FUNCTION__, e.what());
        ret = -1;
    }
    catch(...)
    {
        gLog.Add("[%s] sqlite3_exec err catch", __FUNCTION__);
        ret = -1;
    }

    if(ret != SQLITE_OK)
    {
        gLog.Add("[%s] sqlite3_exec[%s] fail err:[%s]",
              __FUNCTION__, sql_cmd, err_msg);
        sql_free(err_msg);
        return -1;
    }
    return 0;
}

const char* CSqlite::sql_err_msg()
{
    const char* p = pErr;
    if(pSqlDb == NULL)
    {
        return p;
    }

    try
    {
        p = sqlite3_errmsg(pSqlDb);
    }
    catch(...)
    {
        gLog.Add("[%s] sqlite3_errmsg err catch", __FUNCTION__);
    }
    return p;
}

void CSqlite::sql_free(void *err_msg)
{
    if(err_msg == NULL)
    {
        return;
    }

    try
    {
        sqlite3_free(err_msg);
    }
    catch(...)
    {
        gLog.Add("[%s] sqlite3_free err catch", __FUNCTION__);
    }
}

int CSqlite::sql_test()
{
    int ret = 0;
    string sql_cmd;

    ret = sql_open();
    if(ret != 0)
    {
        return -1;
    }

    //    sql_cmd = "create table " + strTableName
    //            + "tb_test(id integer primary key autoincrement,name varchar(100))";

    //    sql_cmd = "";
    //    sql_cmd += "begin;\n";
    //    for (int i = 0; i < 100; i++)
    //    {
    //        sql_cmd += "insert into " + strTableName + "tb_test values(null,'heh');\n";
    //    }
    //    sql_cmd += "commit;";

    //    ret = sql_exe(sql_cmd.c_str());
    //    if (ret != SQLITE_OK)
    //    {
    //        return -3;
    //    }

    sql_cmd = "select * from " + strTableName;
    ret = sql_exe(sql_cmd.c_str(), callback_select, NULL);
    if (ret != SQLITE_OK)
    {
        return -4;
    }

    sql_close();
    return 0;
}

