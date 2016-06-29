#pragma once
#include <string>
#include <sstream>    //使用stringstream需要引入这个头文件  
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
*	字符编码转换类
*/
class CEncodingUtil
{
private:

public:
	CEncodingUtil();
	~CEncodingUtil();
	static const char * CStringToChar(CString str);			//将CString 转换为Char*
	static string CStringToString(CString str);				//将CString 转换为string
	static double StringToNum(string str);					//将CString 转换为数字
};

