#pragma once
#include "TempretureImpl.h"
#include <vector>
#include "TempData.h"
using namespace std;
class CTempretureImpl
{
public:
	CTempretureImpl();
	~CTempretureImpl();
	//插入一组温度数据到数据库
	static void InsertDataToMySql(vector<CTempData*> tempvec);
	//插入单个温度数据到数据库
	static void InsertDataToMySql(CTempData* temp);
};

