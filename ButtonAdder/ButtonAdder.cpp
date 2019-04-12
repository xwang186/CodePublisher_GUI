/////////////////////////////////////////////////////////////////////
// ButtonAdder.cpp                                                 //
//                                                                 //
// Xiaohan Wang, Spring 2019                                       //
/////////////////////////////////////////////////////////////////////

#include "ButtonAdder.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include"../AbstractSyntaxTree/AbstrSynTree.h"
#define COMMENTS_HEADER "<div class=\"toggleCommentDisplay\">"
#define CLASS_HEADER "<div class=\"toggleClassDisplay\">"
#define METHOD_HEADER "<div class=\"toggleMethodDisplay\">"
#define DIV_TEMP "</div>"
#include "../Parser/Parser.h"
#include "../Parser/ConfigureParser.h"
using namespace CodeAnalysis;
//<---Create a ConvOp object---->
ConvOp::ConvOp(OP_TYPE t, int line)
{
	this->type = t;
	this->lineNumber = line;
}
ConvOp::~ConvOp()
{
}
//<----To active sort function for list<ConvOp>---->
bool compare_nocase(ConvOp op1, ConvOp op2) {
	return (op1.getLineNumber() < op2.getLineNumber());
};
//<---Get all the target elements of class and functions and saves them into a list--->
static void TreeWalkSet(ASTNode* pItem, std::list<ConvOp>& oplist)
{
	std::list<ConvOp*> myList;
	static std::string path;
	static size_t indentLevel = 0;
	//int lineNumber = pItem->startLineCount_;
	if (pItem->type_ == "class") {
		ConvOp m1(OP_CLASS, pItem->startLineCount_);
		oplist.push_back(m1);
		ConvOp m2(OP_DIV, pItem->endLineCount_);
		oplist.push_back(m2);
	}
	else if (pItem->type_ == "function") {
		ConvOp m1(OP_METHOD, pItem->startLineCount_);
		oplist.push_back(m1);
		ConvOp m2(OP_DIV, pItem->endLineCount_);
		oplist.push_back(m2);
	}

	auto iter = pItem->children_.begin();
	++indentLevel;
	while (iter != pItem->children_.end())
	{
		TreeWalkSet(*iter, oplist);
		++iter;
	}
	--indentLevel;
	return;
};
//<----To find comments----->
string commentIt(string p) {
	int singcom=p.find("//");
	string m = COMMENTS_HEADER;
	string d = DIV_TEMP;
	if (singcom >= 0)
	{	

		return p.substr(0,singcom)+m + p.substr(singcom,p.length()) + d;
	}
	else {
		int mulcomstart = p.find("/*");
		if (mulcomstart >=0)
		{
			return p.substr(0, mulcomstart)+m + p.substr(mulcomstart,p.length());
		}
		int mulcomend = p.find("*/");
		if (mulcomend >= 0)
		{
			return p.substr(0, mulcomend) + d + p.substr(mulcomend, p.length());
		}
		return p;
	}
}
//<-----Return the element div list----->
std::list<ConvOp> ButtonAdder::getTree(std::string filepath) {
	ConfigParseForCodeAnal configure;
	std::string fileSpec = FileSystem::Path::getFullFileSpec(filepath);
	Parser* pParser = configure.Build();
	std::string name;
	ASTNode* pGlobalScope = NULL;
	std::list<ConvOp> opList;
	try {
		if (pParser)
		{
			name = FileSystem::Path::getName(fileSpec);
			std::cout << "name is " << name << "\n";
			if (!configure.Attach(fileSpec))
			{
				std::cout << "\n  could not open file " << name << std::endl;
			}
		}
		else
		{
			std::cout << "\n\n  Parser not built\n\n";
		}
		Repository* pRepo = Repository::getInstance();
		pRepo->package() = name;
		while (pParser->next())
		{
			pParser->parse();
		}
		std::cout << "\n";
		pGlobalScope = pRepo->getGlobalScope();
		complexityEval(pGlobalScope);
		TreeWalkSet(pGlobalScope, opList);
		//cout << " Value is " << opList.front().getLineNumber() << endl;
	}
	catch (std::exception & ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
		std::cout << "\n  exception caught at line " << __LINE__ << " ";
		std::cout << "\n  in package \"" << name << "\"";
	}
	std::cout << "\n";
	return opList;
}
//<-----Build div elements using a given ConvpOp list----->
void ButtonAdder::ConvertUsingOperations(std::string filepath, std::list<ConvOp> p, std::string outpath)
{
	stringstream buffer;
	p.sort(compare_nocase);
	ifstream in(filepath);
	if (!in.is_open())	cout << "Error opening file" << filepath << " from " << filepath;
	std::string line;
	int number = 1;
	ConvOp* op = NULL;
	if (!p.empty())
	{
		op = &(p.front());
		std::cout << "OP TYPE IS :" << op->getType() << endl;
	}
	while (getline(in, line)){
		buffer << commentIt(line) << endl;
		if (op != NULL) {
			while (number == op->getLineNumber())			{
				string newLine;
				switch (op->getType())				{
				case OP_DIV: newLine = DIV_TEMP;			break;
				case OP_COMMON: newLine = COMMENTS_HEADER;	break;
				case OP_CLASS: newLine = CLASS_HEADER;		break;
				case OP_METHOD:	newLine = METHOD_HEADER;	break;
				default: newLine = "";				break;
				}
				buffer << newLine << endl;
				p.pop_front();
				if (p.empty()) {
					op = NULL;
					break;
				}
				else {
					op = &(p.front());
				}
			}
		}
		number++;
	}
	while (!p.empty()) {
		buffer << DIV_TEMP << endl;
		p.pop_front();
	}
	in.close();
	ofstream out(outpath);
	if (!out.is_open())	cout << "Error opening file" << outpath ;
	out << buffer.str();
	out.close();
}
//<-----An automatic function for the transfer----->
void ButtonAdder::transfer(string path,string out) {
	list<ConvOp> p = getTree(path);
	ButtonAdder::ConvertUsingOperations(path, getTree(path), out);
}
#ifdef TEST_BUTTONADDER_M
//To see the result, you can open Convert.h.html_M.html file in an editor 
// and you will find that the class div is added successfully.
int main() {
	string p = "* hello world!";
	int mulcomstart = p.find("/*");
	cout << "Index is "<<mulcomstart;
	string path = "../TestPages/testForButtonAdder/Convert.h.html";
	ButtonAdder::transfer(path, path + "_M.html");
	
}
#endif // TEST_LINKADDER