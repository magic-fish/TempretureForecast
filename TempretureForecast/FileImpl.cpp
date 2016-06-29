#include "stdafx.h"
#include "FileImpl.h"
#include <fstream>
#include "EncodingUtil.h"
#include "StringSplitUtil.h"

CFileImpl::CFileImpl()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	sprintf_s(m_filename, "D://file//%02d-%02d-%02d", sys.wHour, sys.wMinute, sys.wSecond);
}


CFileImpl::~CFileImpl()
{
}
BOOL CFileImpl::WriteTempreturetoFile(vector<CTempData*> tempvec)
{
	fstream dataFile;                    //�����ļ���������
	dataFile.open(m_filename, ios::out);   //����һ���ļ�
	if (!dataFile)                        //����ļ��Ƿ񴴽��ɹ�
	{
	}
	for (int i = 0; i < tempvec.size(); i++)
	{
		dataFile << tempvec.at(i)->ToData() << endl;
	}
	dataFile.close();                   //�ر��ļ������û�ر��ļ������ݽ�û��д�롣��Ϊ��һ������        
	return TRUE;
}

vector<CTempData*> CFileImpl::ReadTempretureFromFile(string filePath)
{
	vector<CTempData*>pData;
	ifstream infile;
	infile.open(filePath);      //ע���ļ���·��  
	//����Ǹ��ܴ�Ķ��д洢���ı����ļ�������ô����  
	char buf[1024];                //��ʱ�����ȡ�������ļ�����  
	string message;
	if (infile.is_open())          //�ļ��򿪳ɹ�,˵������д�������  
	{
		while (infile.good() && !infile.eof())
		{
			memset(buf, 0, 1024);
			infile.getline(buf, 1204);
			message = buf;//������ܶ�message��һЩ����  
			if (message == "")
			{
				break;
			}
			string pattern = ":";
			vector<string> str =  CStringSplitUtil::split(message, pattern);

			CTempData* data = new CTempData();
			data->m_date.wHour = CEncodingUtil::StringToNum(str[0]);
			data->m_date.wMinute = CEncodingUtil::StringToNum(str[1]);
			data->m_date.wSecond = CEncodingUtil::StringToNum(str[2]);
			data->m_date.wMilliseconds = CEncodingUtil::StringToNum(str[3]);
			data->m_temperature = CEncodingUtil::StringToNum(str[4]);
			pData.push_back(data);
		}
		infile.close();
	}
	return pData;
}

vector<CTempData*> CFileImpl::Read(string str)
{
	return ReadTempretureFromFile(str);
}
void CFileImpl::Write(vector<CTempData*> pTemp)
{
	WriteTempreturetoFile(pTemp);
}