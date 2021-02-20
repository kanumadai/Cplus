
#include "CFileRead.h"
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <stdio.h>
#include <iostream>

#include <cstring>
using namespace std;


CFileRead::CFileRead()
{
	strFileName = "";

}


CFileRead::~CFileRead()
{
}

int CFileRead::ReadFile( string &strFileName,vector<string> &vFileData) {

ifstream file;
vector<string> vStr;
int a[9] = { 0 };
int iRet = 0;
try {
	file.open(strFileName);
}
catch (...) {
	cout << "error";
}
string strFileLine;
while (getline(file, strFileLine)) {
	 //split(strFileLine, ",",vStr);
	 if (vStr.size() > 9) {
		 cout << "line num >9";
	 }

	 vFileData.push_back(strFileLine);

	 //for (int i = 0; i < 9; i++) {
		// a[i] = stoi(vStr.data.at(i));
	 //}
	 //dataMap.insert(map<string, int*>::value_type(strFileLine, a));
}
file.close();
return iRet;
}

int CFileRead::split(const string &str, const string &delim, vector<int> &vStrOneLine,string &strOutStr) {
	int iRet = 0;

	string::size_type pos1, pos2;
	pos2 = str.find(delim);
	pos1 = 0;
	strOutStr.clear();
	while (string::npos != pos2)
	{
		vStrOneLine.push_back(stoi(str.substr(pos1, pos2 - pos1)));
		strOutStr = strOutStr+str.substr(pos1, pos2 - pos1);
		pos1 = pos2 + delim.size();
		pos2 = str.find(delim, pos1);
	}
	if (pos1 != str.length()) {
		vStrOneLine.push_back(stoi(str.substr(pos1)));
		strOutStr = strOutStr + str.substr(pos1);
	}
	return iRet;
}

//int CFileRead::split(const string &str, const string &delim, vector<int> &vStrOneLine) {
//	int iRet = 0;
//	char *buf;
//	if ("" == str) return 1;
//	//ﾏﾈｽｫﾒｪﾇﾐｸ鋙ﾄﾗﾖｷ逸ｮｴﾓstringﾀ獎ﾍﾗｪｻｻﾎｪchar*ﾀ獎ﾍ  
//	char * strs = new char[str.length() + 1]; //ｲｻﾒｪﾍ・ﾋ  
//	strcpy_s(strs,strlen(strs)+1, str.c_str());
//
//	char * d = new char[delim.length() + 1];
//	strcpy_s(d,strlen(d)+1, delim.c_str());
//
//	char *p = strtok_s(strs, d, &buf);
//	while (p) {
//		string s = p; //ｷﾖｸ鋙ﾃｵｽｵﾄﾗﾖｷ逸ｮﾗｪｻｻﾎｪstringﾀ獎ﾍ  
//		vStrOneLine.push_back(stoi(s)); //ｴ貶・盪鈹ﾗ・ 
//		p = strtok_s(NULL, d,&buf);
//	}
//
//	 strs =NULL;
//	 d = NULL;
//	return iRet;
//}