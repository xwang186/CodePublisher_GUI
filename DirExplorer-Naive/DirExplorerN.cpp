/////////////////////////////////////////////////////////////////////
// DirExplorerN.cpp                                                //
//                                                                 //
// Xiaohan Wang, Jim Fawcett, Spring 2019                          //
/////////////////////////////////////////////////////////////////////
#pragma once


#include "DirExplorerN.h"
using namespace FileSystem;
// -----< function Usage to show usage information >---------------

#include<conio.h>
#ifdef TEST_DIREXPLORERN  // only compile the following when defined
int main(int argc, char* argv[])
{
	Title("Demonstrate DirExplorer-Naive, " + DirExplorerN::version());

	ProcessCmdLine pcl(argc, argv);

	preface("Command Line: ");
	pcl.showCmdLine();
	putline();

	if (pcl.parseError())
	{
		pcl.usage();
		std::cout << "\n\n";
		return 1;
	}

	DirExplorerN de(pcl.path());
	for (auto patt : pcl.patterns())
	{
		de.addPattern(patt);
	}

	if (pcl.hasOption('s'))
	{
		de.recurse();
	}

	de.search();
	de.showStats();

	std::cout << "\n\n";
	return 0;
}

#endif
