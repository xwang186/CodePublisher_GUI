#include "CodePublisher.h"
#include "../Filter/Filter.h"
#include "../Convert/Convert.cpp"
#include "../DirExplorer-Naive/DirExplorerN.h"



vector<string> CodePublisher::getMatchedFileName(vector<string> filenames, string reg)
{
	Filter filter(reg);
	return filter.RegIt(filenames);
}

void CodePublisher::ConvertFile(string filepath)
{
	Convert::FinialConvert(filepath);
}
void ConvertFilesOfDic(string path, string reg);
void CodePublisher::ConvertUnderPath(string filepath,string reg) {

	ConvertFilesOfDic(filepath,reg);
	
}
vector<string> CodePublisher::getFiles(string path) {
	return FileSystem::Directory::getFiles(path);
}
vector<string> CodePublisher::getCodes(string path)
{
	vector<string> result1= FileSystem::Directory::getFiles(path,"*.cpp");
	vector<string> result2 = FileSystem::Directory::getFiles(path, "*.h");
	for (string i : result2) result1.push_back(i);
	return result1;
}

void CodePublisher::ConvertFilesOfDic(string path,string reg) {
	vector<string> filenames = getCodes(path);
	vector<string> dics = FileSystem::Directory::getDirectories(path);
	Filter filter(reg);
	std::vector<std::string> matches = filter.RegIt(filenames);
	for (std::string i : matches) Convert::FinialConvert(path+"/"+i);
	for (std::string dic : dics) {
		if (dic!="."&&dic!="..")
		{
			ConvertFilesOfDic(path + "/" + dic, reg);
		}
	}
}
CodePublisher::CodePublisher()
{
}


CodePublisher::~CodePublisher()
{
}
