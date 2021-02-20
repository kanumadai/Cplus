#include <io.h>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class FindFiles
{
public:
    FindFiles();
    ~FindFiles();
    void test();
    void getFiles(string path, vector<string>& files);
private:

};

FindFiles::FindFiles()
{
}

FindFiles::~FindFiles()
{
}
void FindFiles::getFiles(string path, vector<string>& files)
{
    //文件句柄
    intptr_t hFile = 0;
    //文件信息
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //如果是目录,迭代之
            //如果不是,加入列表
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}



void FindFiles::test() {

    string filePath = "G:\\100_Projects\\05_C++";
    vector<string> files;

    //获取该路径下的所有文件
    getFiles(filePath, files);

    char str[30];
    int size = files.size();
    for (int i = 0; i < size; i++)
    {
        cout << files[i].c_str() << endl;
    }
}