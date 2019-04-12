#pragma once
/*
 * Package Operations:
 * -------------------
 * This package contains 2 files --Convert.h and Convert.cpp.
 * This package implements a function to convert a file into html file in a created folder "convertedPages"
 * To use this package, use the function ConvertIt to convert.
 * Two arguments should be given: first one is the target file name, the second one is the path that the file is under.
 *
 * Required Files:
 * ---------------
 * Convert.h  Convert.cpp
 *
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 4 Feb 2019
 *
 */
/////////////////////////////////////////////////////////////////////
// Convert.h                                                       //
//                                                                 //
// Xiaohan Wang, Jim Fawcett, Spring 2019                          //
/////////////////////////////////////////////////////////////////////

#include <string>
#include <list>
using namespace std;
class Convert {
public:
	Convert();
	static void ConvertIt(const std::string& filename, std::string filepath,string targetpath);
	static void WriteTemplate(string filename,string filepath, string targetpath);
	static void FinialConvert(string filepath);
};

