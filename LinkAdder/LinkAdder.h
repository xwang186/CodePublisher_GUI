#pragma once
/*
 * Package Operations:
 * -------------------
 * This package contains 2 files --LinkAdder.h and LinkAdder.cpp.
 * This package implements a function to add link element into an html file
 * To use this package, use AddLink()
 * Two arguments should be given: first one is the target file name, the second one is the path that the file is under.
 *
 * Required Files:
 * ---------------
 * LinkAdder.h  LinkAdder.cpp  
 *
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 4 Feb 2019
 *
 */
 /////////////////////////////////////////////////////////////////////
 // LinkAdder.h                                                     //
 //                                                                 //
 // Xiaohan Wang, Spring 2019                                       //
 /////////////////////////////////////////////////////////////////////

#include<map>
#include<string>
using namespace std;
class LinkAdder {

public:
	static void AddLink(std::string filepath,string targetpath);
};