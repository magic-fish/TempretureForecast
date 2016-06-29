#pragma once
#include <vector>
using namespace std;
class CStringSplitUtil
{
public:
	CStringSplitUtil();
	~CStringSplitUtil();
	//·Ö¸î×Ö·û´®
	static vector<string> split(string str, string pattern);
};

