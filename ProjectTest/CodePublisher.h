#pragma once
#include<vector>
#include<string>
using namespace std;
class CodePublisher
{
public:
	vector<string> getMatchedFileName(vector<string>filenames,string reg);
	void ConvertFile(string filepath);
	void ConvertUnderPath(string filepath,string regu);
	vector<string> getFiles(string path);
	vector<string> getCodes(string path);
	void ConvertFilesOfDic(string path, string reg);
	CodePublisher();
	~CodePublisher();
};

