#include "stdafx.h"
#include "DbConnect.h"


CDbConnect* CDbConnect::conn = NULL;

CDbConnect::CDbConnect()
{
	db = new CDatabase();
	connect();
}


CDbConnect::~CDbConnect()
{
	db->Close(); 
	delete db;
}


CDbConnect* CDbConnect::GetInstance()
{
	if (conn == NULL)
		conn = new CDbConnect();
	return conn;
}
int CDbConnect::connect()
{
	try
	{
		if (db->Open(NULL, FALSE, FALSE, L"ODBC;Dsn=TempretureForecast;uid=root;PWD=....1234"))
		{

		}
	}
	catch (CDBException* e)
	{
		AfxMessageBox(e->m_strError);
		e->Delete();
	}
	return 0;
}


CDatabase* CDbConnect::getDatabase()
{
	return db;
}
