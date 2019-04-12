#pragma once
/*
 * Package Operations:
 * -------------------
 * This package contains 2 files --ButtonAdder.h and ButtonAdder.cpp.
 * This package implements a function to add buttopn div element into an html file 
 * To use this package, use getTree() and ConvertUsingOperations()
 * Two arguments should be given: first one is the target file name, the second one is the path that the file is under.
 *
 * Required Files:
 * ---------------
 * Convert.h  Convert.cpp  FileSystem.h  FileSystem.cpp
 *
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 4 March 2019
 *
 */
 /////////////////////////////////////////////////////////////////////
 // ButtonAdder.h                                                   //
 //                                                                 //
 // Xiaohan Wang, Spring 2019                                       //
 /////////////////////////////////////////////////////////////////////
#include<string>
#include<list>
using namespace std;
enum  OP_TYPE { OP_COMMON, OP_METHOD, OP_CLASS, OP_DIV };
class ConvOp {
private:
	OP_TYPE type;
	int lineNumber;
public:
	ConvOp(OP_TYPE t, int line);
	~ConvOp();
	bool operator<(const ConvOp& conv) const { return lineNumber < conv.lineNumber; }
	int getLineNumber() { return lineNumber; }
	int getType() { return type; }
};
class ButtonAdder {
public:
	static std::list<ConvOp> getTree(std::string filepath);
	static void ConvertUsingOperations(std::string filepath, std::list<ConvOp> p, std::string outpath);
	static void transfer(string path, string out);
};