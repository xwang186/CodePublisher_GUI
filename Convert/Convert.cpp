/////////////////////////////////////////////////////////////////////
// Convert.cpp                                                     //
//                                                                 //
// Xiaohan Wang, Jim Fawcett, Spring 2019                          //
/////////////////////////////////////////////////////////////////////

#include <regex>
#include "Convert.h"


#include <fstream>
#include <iostream>

#include <sstream>
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
using namespace std;
// -----< function replacement to replace one elmenet in a string with anthor element >---------------
string& replacement(string& str, const string& old_value, const  string& new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length())
	{
		if ((pos = str.find(old_value, pos)) != string::npos)
		{
			str.replace(pos, old_value.length(), new_value);
		}
		else { break; }
	}
	return   str;
}
// -----< function WriteTemplete write html headers and ends to a content, and highlight the file name >---------------
void Convert::WriteTemplate(string filename,string filepath,string targetpath) {
	ifstream head("../templateHead.txt");
	ifstream end("../templateEnd.txt");
	ifstream content(filepath);
	if (!head.is_open()|| !end.is_open())
	{
		cout << "Error opening head or end file!"; exit(1);
	}
	stringstream bufferH;
	bufferH << head.rdbuf();
	stringstream bufferE;
	bufferE << end.rdbuf();
	stringstream bufferContent;
	bufferContent << content.rdbuf();
	content.close();
	head.close();
	end.close();
	ofstream out(targetpath);
	out << (bufferH.str() + "<h3>" + filename + "</h3>" +"<pre>"+ bufferContent.str()+"</pre>" + bufferE.str());
}

Convert::Convert()
{
}
#include<direct.h>
#include<conio.h>

// -----< function ConvertIt to do all the jobs of translating one file into html format >---------------
void Convert::ConvertIt(const std::string& filename,std::string filepath, string targetpath)
{

	stringstream buffer;
	ifstream in(filepath);

	ofstream out(targetpath);
	if (!in.is_open())
	{
		cout << "Error opening file"<<filename << " from "<<filepath; 
	}
	
	if (!out.is_open())
	{
		cout << "Error Creating file"<<filename; 
	}
	buffer << in.rdbuf();
	in.close();
	string contents = buffer.str();
	contents = replacement(contents, "<", "&lt");
	contents = replacement(contents, ">", "&gt");
	//contents = replacement(contents, "\n", "<br>");
	out << contents; 
	out.close();
}
void showFile(string path) {
	ifstream in(path);
	stringstream s;
	s<< in.rdbuf();
	in.close();
	cout << s.str()<<endl;
}
#include "../ButtonAdder/ButtonAdder.h"
#include "../LinkAdder/LinkAdder.h"
//<-------All the steps inside one convert, using "convertedPages and default folder"------------>
void Convert::FinialConvert(string filepath) {
	list<ConvOp> t= ButtonAdder::getTree(filepath);
	string filename = FileSystem::Path::getName(filepath);
	string foldername = "..\\convertedPages";
	int k = _mkdir(foldername.c_str());
	string htmlFilePath = "../convertedpages/" + filename + ".html";
	Convert::ConvertIt(filename, filepath, htmlFilePath);
	ButtonAdder::ConvertUsingOperations(htmlFilePath,t,htmlFilePath);
	LinkAdder::AddLink(htmlFilePath,htmlFilePath);
	Convert::WriteTemplate(filename,htmlFilePath,htmlFilePath);
}
#ifdef TEST_CONVERT_M  // only compile the following when defined
int main() {
	string filepath = "../TestPages/testForConvert/Parser.cpp";
	list<ConvOp> t = ButtonAdder::getTree(filepath);
	string filename = FileSystem::Path::getName(filepath);
	string htmlFilePath = "../TestPages/testForConvert/generated/" + filename + ".html";
	Convert::ConvertIt(filename, filepath,htmlFilePath);
	ButtonAdder::ConvertUsingOperations(htmlFilePath, t, htmlFilePath);
	LinkAdder::AddLink(htmlFilePath, htmlFilePath);
	Convert::WriteTemplate(filename, htmlFilePath, htmlFilePath);
	std::cout << "Convert Finished!";
	return 0;
}
#endif