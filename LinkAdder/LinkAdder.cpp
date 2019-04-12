#include "LinkAdder.h"
#pragma once
/////////////////////////////////////////////////////////////////////
// LinkAdder.cpp                                                   //
//                                                                 //
// Xiaohan Wang, Jim Fawcett, Spring 2019                          //
/////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
//<------Find the exact content xxx inside of a #include "xxx"------->
string getContent(string p) {
	string result = "";
	int count = 0;
	while (p.at(count) != '\"') {
		count++;
		if (count == (p.length() - 2)) return "";
	}
	count++;
	while (p.at(count) != '\"') {
		if (count == (p.length() - 1)) return "";
		result += p.at(count);
		count++;
	}
	return result;
}
//<----------Add the link element<href>------------>
void LinkAdder::AddLink(std::string filepath,string targetpath)
{
	stringstream buffer;
	ifstream in(filepath);
	if (!in.is_open())
	{
		cout << "Error opening file from " << filepath;
	}
	buffer << in.rdbuf();
	in.close();
	string contents = "";
	string line;
	ofstream out(targetpath);
	string linkHeader1 = "<a href=\"";
	string linkHeader2 =  "\">";
	string linkEnder = "</a>";
	std::regex matcher("(#include)( *)(\")(.*)");
	while (std::getline(buffer,line)) {
		if (regex_match(line, matcher)) {
			string p = "#include \""+linkHeader1 + FileSystem::Path::getName(getContent(line))+ ".html" +linkHeader2 + getContent(line) +"\""+ linkEnder;
			out << p <<endl;
			std::cout << "Find one link: " << getContent(line) << endl;
		}
		else {
			out << line<<endl;
		}
	}
	out.close();

}
#ifdef TEST_LINKADDER_M

int main() {
	LinkAdder::AddLink("../TestPages/FileSystem.cpp.html", "../TestPages/FileSystem.cpp_M.html");
}
#endif // TEST_LINKADDER
