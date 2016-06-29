#pragma once
#include "DataStorage.h"
class CSourceWRFactory
{
public:
	CSourceWRFactory();
	~CSourceWRFactory();

	//创建文件存储的工厂类
	static CDataStorage * CreateSourceWR(string name);
};

