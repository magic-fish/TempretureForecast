#pragma once
#include "TempretureImpl.h"
#include <vector>
#include "TempData.h"
#include "DataStorage.h"
using namespace std;
class CTempretureImpl
	:public CDataStorage
{
private:

	void InsertDataToMySql(vector<CTempData*> tempvec);		//插入一组温度数据到数据库
	void InsertDataToMySql(CTempData* temp);					//插入单个温度数据到数据库
public:
	CTempretureImpl();
	~CTempretureImpl();
	
	vector<CTempData*> Read(string str);
	void Write(vector<CTempData*> pTemp);
};

