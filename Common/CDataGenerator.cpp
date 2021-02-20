#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <ComDef.h>

#include "CDataGenerator.h"
using namespace std;





CDataGenerator::CDataGenerator()
{
}


CDataGenerator::~CDataGenerator()
{
}

int CDataGenerator::NineByNine(map<string, string> &NineByNineMap) {
	int iRet = 0;
	map<string, string>::iterator testmapIter;
	int a[3][3] = { 0 };
	int b = 0;
	int i = 0, j = 0;
	b =(int)floor((rand() % 100 / (double)101) * 10);
	bool flag = false;
	int ttt = 0;
	string strKey = "";
	int count = 1;
	vector<string> fileVec;
	int matrix[9][9] = { 0 };
	int out3x3[3][3] = { 0 };
	int inital = 1;

	for (int q = 1; q < 10; q++) {
		a[0][0] = q;
		cout << q;
		do {
			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					if (i == 0 && j == 0) {
						//std::cout << a[i][j] << ",";
						continue;
					}

					do {

						b = (int)floor((rand() % 100 / (double)101) * 10);
						if (b > 0) {
							for (int m = 0; m <= i; m++) {
								if (i > 0 && m < i)
									ttt = 3;
								else
									ttt = j;
								for (int n = 0; n < ttt; n++) {
									if (b == a[m][n]) {
										flag = true;
										break;
									}
									else {
										flag = false;
									}
								}
								if (flag == true)
									break;
							}
							a[i][j] = b;
						}
						else {
							flag = true;
						}
					} while (flag);


					//std::cout << a[i][j] << ",";

				}
				//std::cout << "\n";
			}

			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					if (i == 2 && j == 2) {
					strKey = strKey + to_string(a[i][j]);
					}
					else {
					strKey = strKey + to_string(a[i][j]) + ",";
					}
				}
			}

			testmapIter = NineByNineMap.find(strKey);
			if (testmapIter != NineByNineMap.end()) {
				strKey = "";
			}
			else {
				NineByNineMap.insert(map<string, string>::value_type(strKey, strKey));
				count++;
				strKey = "";
			}
			if (count > 8 * 7 * 6 * 5 * 4 * 3 * 2)
				break;
		} while (true);
		count = 1;

	}

	return iRet;
}