#include "stdafx.h"
#include "TempretureImpl.h"
#include "DbConnect.h"

CTempretureImpl::CTempretureImpl()
{
}


CTempretureImpl::~CTempretureImpl()
{
}


void CTempretureImpl::InsertDataToMySql(vector<CTempData*> tempvec)
{
	CDatabase *db = CDbConnect::GetInstance()->getDatabase();
	if (!db->BeginTrans())
		return;
	for (int i = 0; i < tempvec.size(); i++){
		CString sql;
		sql.Format(_T("insert into tempreture(tempreture,time) values('%s','%s')"), tempvec.at(i)->GetTemperature(), tempvec.at(i)->GetTime());
		db->ExecuteSQL(sql);
	}
	db->CommitTrans();

}
void CTempretureImpl::InsertDataToMySql(CTempData* temp)
{
	CDatabase *db = CDbConnect::GetInstance()->getDatabase();
	if (!db->BeginTrans())
		return;
	int n = 3;
	while (n--)
	{
		CString sql;
		sql.Format(_T("insert into tempreture(tempreture,time) values('%s','%s')"), temp->GetTemperature(), temp->GetTime());
		db->ExecuteSQL(sql);
	}
	db->CommitTrans();

}


vector<CTempData*> CTempretureImpl::Read(string sql)
{
	vector<CTempData*> pdata ;
	return pdata;
}
void CTempretureImpl::Write(vector<CTempData*> pTemp)
{
	InsertDataToMySql(pTemp);
}