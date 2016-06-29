#pragma once
#include <string>
#include <sstream>    //ʹ��stringstream��Ҫ�������ͷ�ļ�  
using namespace std;
template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}
/*
*	�ַ�����ת����
*/
class CEncodingUtil
{
private:

public:
	CEncodingUtil();
	~CEncodingUtil();
	static const char * CStringToChar(CString str);			//��CString ת��ΪChar*
	static string CStringToString(CString str);				//��CString ת��Ϊstring
	static double StringToNum(string str);					//��CString ת��Ϊ����
};

