#pragma once
#include <vector>
using namespace std;
class CStringSplitUtil
{
public:
	CStringSplitUtil();
	~CStringSplitUtil();
	static vector<string> split(string str, string pattern);
};

