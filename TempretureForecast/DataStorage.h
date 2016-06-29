#pragma once
#include <string>
#include <vector>
#include "TempData.h"
using namespace std;
class CDataStorage
{
public:
	CDataStorage();
	~CDataStorage();
	virtual vector<CTempData*> Read(string str) = 0;
	virtual void Write(vector<CTempData*> pTemp) = 0;
};

