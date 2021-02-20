
#define _CRI_SCURE_NO_WARNNINGS

#include <iostream>
#include <map>
#include <string.h>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <functional>
#include <time.h>
#include <deque>
#include <numeric>
#include <string>

using namespace std;




//class person
class Person {

public:
	Person(string name, int age) {
		this->m_Name = name;
		this->m_Age = age;
	}

	bool operator==(const Person& p) {
		return this->m_Name == p.m_Name && this->m_Age == p.m_Age;
	}

	int m_Age;
	string m_Name;
};

/// <summary>
/// find 
/// </summary>
void test1() {

	vector<Person> v;

	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);

	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);


	vector<Person>::iterator vi = find(v.begin(), v.end(), p1);
	if (vi != v.end()) {
		cout << (*vi).m_Name << "  " << (*vi).m_Age << endl;
	}

}


class MyClass:public binary_function<Person*,Person*,bool>
{
public:
	bool operator()(Person* p1, Person* p2) const {
		return (p1->m_Name == p2->m_Name && p1->m_Age == p2->m_Age);
	}

private:

};


/// <summary>
/// find if
/// </summary>
void test2() {

	vector<Person*> v;

	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);

	v.push_back(&p1);
	v.push_back(&p2);
	v.push_back(&p3);
	v.push_back(&p4);


	vector<Person*>::iterator vi = find_if(v.begin(), v.end(), bind2nd(MyClass(),&p1));
	if (vi != v.end()) {
		cout << (*vi)->m_Name << "  " << (*vi)->m_Age << endl;
	}

}

/// <summary>
/// 
/// </summary>
/// <returns></returns>

void test3() {
	vector<int> v1;
	vector<int> v2;

	for (int i = 0; i < 10; i++) {
		v1.push_back(i);
		v2.push_back(i + 1);
	}

	vector<int> vt;
	vt.resize(v1.size() + v2.size());
	merge(v1.begin(), v1.end(), v2.begin(), v2.end(), vt.begin());

	for_each(vt.begin(), vt.end(), [](int val) {cout << val << endl; });

}

void test4() {
	vector<int> v1;

	for (int i = 0; i < 10; i++) {
		v1.push_back(i);
	}

	random_shuffle(v1.begin(), v1.end());

	for_each(v1.begin(), v1.end(), [](int val) {cout << val << endl; });

}


class Speaker {

public:
	string m_Name;
	int m_Score[3];


};


void createSpeaker(vector<int>& v, map<int, Speaker> & speakerMap) {

	string nameSeed = "ABCDEFGHIJKLMNOPQRSTUVWX";
	for (int i = 0; i < 24; i++) {
		string name = "Speaker";
		name += nameSeed[i];

		Speaker sp;
		sp.m_Name = name;

		for (int j = 0; j < 3; j++) {
			sp.m_Score[j] = 0;
		}

		v.push_back(i + 100);

		speakerMap.insert(make_pair(i+100, sp));
	}

}

void speakerDraw(vector<int> &v) {
	random_shuffle(v.begin(), v.end());
}

void speechConest(int index, vector<int>& v, map<int, Speaker>& speakerMap, vector<int>&v1) {

	multimap<int, int, greater<int>> groupMap;
	int num = 0;
	int group = 1;
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		num++;
		deque<int> d;
		//speech score by 10 judges
		for (int i = 0; i < 10; i++) {
			int score = rand() % 41 + 60;;
			d.push_back(score);
		}
		//sort 
		sort(d.begin(), d.end());

		//delete max and min
		d.pop_back();
		d.pop_front();

		//average 
		int avg = accumulate(d.begin(), d.end(),0)/d.size();

		speakerMap[(*it)].m_Score[index-1] = avg;

		//tmp map
		groupMap.insert(make_pair(avg, (*it)));

		if (num % 6 == 0) {
			int count = 0;
			cout << "Group:" << group << endl;
			group++;
			for (multimap<int, int, greater<int>>::iterator mit = groupMap.begin(); mit != groupMap.end(); mit++) {
				cout << "Num: " << (*mit).second << " Name: " << speakerMap[mit->second].m_Name << " Score: " << speakerMap[mit->second].m_Score[index - 1] << endl;

				if (count < 3) {
					v1.push_back((*mit).second);
				}
				count++;

			}
			groupMap.clear();
		}
		
	}
}

void showScore(int index, vector<int>& v, map<int, Speaker>& speakes) {

	cout << "Round: " << index << endl;

	for (map<int, Speaker>::iterator it = speakes.begin(); it != speakes.end(); it++) {

		cout << "Numb: " << it->first 
			 << " Name: " << it->second.m_Name 
			 << " Score: " << it->second.m_Score[0]
					<<" "<< it->second.m_Score[1] 
					<< " " << it->second.m_Score[2] << endl;
	}

		cout << "Next Round :" << endl;
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {

		cout << *it << endl;
	}
}

// file operator
int fileOperator()
{

	char data[100];

	// 以写模式打开文件
	ofstream outfile;
	outfile.open("afile.dat");

	outfile << "Writing to the file" << endl;
	outfile << "Enter your name: ";
	cin.getline(data, 100);

	// 向文件写入用户输入的数据
	outfile << data << endl;

	//cout << "Enter your age: ";
	//cin >> data;
	//cin.ignore();

	//// 再次向文件写入用户输入的数据
	//outfile << data << endl;

	// 关闭打开的文件
	outfile.close();

	// 以读模式打开文件
	ifstream infile;
	infile.open("afile.dat");

	cout << "Reading from the file" << endl;
	string str;
	getline(infile, str);



	// 在屏幕上写入数据
	cout << str << endl;


	// 关闭打开的文件
	infile.close();

	return 0;
}


int main()
{
	srand((unsigned int)time(NULL));
//	test1();

//	test2();

//	test3();

	//test4();

	//create no and speakers
	vector<int> v;	//no
	map<int, Speaker> speakes;
	createSpeaker(v, speakes);

	//
	speakerDraw(v);

	//contest
	int index = 1;
	vector<int> v2;
	speechConest(index,v,speakes,v2);


	//show
	showScore(index, v2, speakes);
	//round 2

	speakerDraw(v2); 

	 index = 2;
	vector<int> v3;
	speechConest(index, v2, speakes, v3);


	//show
	showScore(index, v3, speakes);
	//round 3
	speakerDraw(v3);

	index = 3;
	vector<int> v4;
	speechConest(index, v3, speakes, v4);


	//show
	showScore(index, v4, speakes);


}

