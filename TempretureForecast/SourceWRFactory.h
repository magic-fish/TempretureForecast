#pragma once
#include "DataStorage.h"
class CSourceWRFactory
{
public:
	CSourceWRFactory();
	~CSourceWRFactory();

	//�����ļ��洢�Ĺ�����
	static CDataStorage * CreateSourceWR(string name);
};

