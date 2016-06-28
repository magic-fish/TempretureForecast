#include "stdafx.h"
#include "EncodingUtil.h"


CEncodingUtil::CEncodingUtil()
{
}


CEncodingUtil::~CEncodingUtil()
{
}


const char * CEncodingUtil::CStringToChar(CString str)
{
	USES_CONVERSION;
	string stringstr = W2A(str.GetBuffer(0));
	const char* charstr = stringstr.c_str();
	str.ReleaseBuffer();
	return charstr;
}
string CEncodingUtil::CStringToString(CString str){
	USES_CONVERSION;
	string stringstr = W2A(str.GetBuffer(0));
	str.ReleaseBuffer();
	return stringstr;
}

int CEncodingUtil::StringToInt(string str)
{
	char* end;
	int i = static_cast<int>(strtol(str.c_str(), &end, 16));
	return i;
}
double CEncodingUtil::StringToDouble(string str)
{
	cout << str << endl;

	double value = stringToNum<double>(str);
	return value;
}
