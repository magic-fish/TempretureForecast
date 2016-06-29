#pragma once
#include <vector>
#include <string>
#include "TempData.h"
#include "DataStorage.h"
using namespace std;
#define MAX_SIZE  1024
class CFileImpl
	:public CDataStorage
{
private:
	char m_filename[MAX_SIZE];									//初始化保存的文件路径
	BOOL WriteTempreturetoFile(vector<CTempData*> tempvec);		//将温度采集的数据保存到文件中
	vector<CTempData*> ReadTempretureFromFile(string filePath);	//从文件中读取温度数据
public:
	CFileImpl();
	~CFileImpl();
public:
	vector<CTempData*> Read(string str) ;						//重写基类的Read()
	void Write(vector<CTempData*> pTemp);						//重写基类的Write()				
};

