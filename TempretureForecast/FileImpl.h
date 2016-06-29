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
	char m_filename[MAX_SIZE];									//��ʼ��������ļ�·��
	BOOL WriteTempreturetoFile(vector<CTempData*> tempvec);		//���¶Ȳɼ������ݱ��浽�ļ���
	vector<CTempData*> ReadTempretureFromFile(string filePath);	//���ļ��ж�ȡ�¶�����
public:
	CFileImpl();
	~CFileImpl();
public:
	vector<CTempData*> Read(string str) ;						//��д�����Read()
	void Write(vector<CTempData*> pTemp);						//��д�����Write()				
};

