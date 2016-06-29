#pragma once
#include "afxdb.h"
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")
/*
 *数据库连接
 */
class CDbConnect
{
public:
	CDbConnect();

	CDatabase* db;
	static CDbConnect* GetInstance();
private:
	~CDbConnect();
	static CDbConnect *conn;
public:
	int connect();
	CDatabase* getDatabase();
};

