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
	fstream dataFile;                    //定义文件操作对象
	dataFile.open(m_filename, ios::out);   //创建一个文件
	if (!dataFile)                        //检查文件是否创建成功
	{
	}
	for (int i = 0; i < tempvec.size(); i++)
	{
		dataFile << tempvec.at(i)->ToData() << endl;
	}
	dataFile.close();                   //关闭文件。如果没关闭文件的数据将没有写入。因为有一个输入        
	return TRUE;
}

vector<CTempData*> CFileImpl::ReadTempretureFromFile(string filePath)
{
	vector<CTempData*>pData;
	ifstream infile;
	infile.open(filePath);      //注意文件的路径  
	//如果是个很大的多行存储的文本型文件可以这么读：  
	char buf[1024];                //临时保存读取出来的文件内容  
	string message;
	if (infile.is_open())          //文件打开成功,说明曾经写入过东西  
	{
		while (infile.good() && !infile.eof())
		{
			memset(buf, 0, 1024);
			infile.getline(buf, 1204);
			message = buf;//这里可能对message做一些操作  
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