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

	void InsertDataToMySql(vector<CTempData*> tempvec);		//����һ���¶����ݵ����ݿ�
	void InsertDataToMySql(CTempData* temp);					//���뵥���¶����ݵ����ݿ�
public:
	CTempretureImpl();
	~CTempretureImpl();
	
	vector<CTempData*> Read(string str);
	void Write(vector<CTempData*> pTemp);
};

