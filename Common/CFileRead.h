#pragma once

#include <string>
#include <vector>
using namespace std;

class CFileRead 
{
public:
	CFileRead();
	~CFileRead();

	int ReadFile(string &strFileName,vector<string> &vFileData);

	int split(const string &str, const string &delim, vector<int> &vStrOneLine, string &strOutStr);

public:
	string strFileName;
	vector<string> FileData;

	int r1;
	int r2;
	int r3;
	int r4;
	int r5;
	int r6;
	int r7;
	int r8;
	int r9;
};

