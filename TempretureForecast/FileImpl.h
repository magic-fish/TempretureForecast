#pragma once
#include <vector>
#include <string>
#include "TempData.h"
using namespace std;
#define MAX_SIZE  1024
class CFileImpl
{
private:
	char m_filename[MAX_SIZE];
public:
	CFileImpl();
	~CFileImpl();
public:
	BOOL WriteTempreturetoFile(vector<CTempData*> tempvec);
	vector<CTempData*> ReadTempretureFromFile(string filePath);
};

