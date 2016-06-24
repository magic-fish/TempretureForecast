#pragma once
#include "afxdb.h"
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")
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

