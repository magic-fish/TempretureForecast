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
	//����һ���¶����ݵ����ݿ�
	static void InsertDataToMySql(vector<CTempData*> tempvec);
	//���뵥���¶����ݵ����ݿ�
	static void InsertDataToMySql(CTempData* temp);
};

