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
class CEncodingUtil
{
private:

public:
	CEncodingUtil();
	~CEncodingUtil();
	static const char * CStringToChar(CString str);
	static string CStringToString(CString str);
	static int StringToInt(string str);
	static double StringToDouble(string str);
};

