/////////////////////////////////////////////////////////////////////////////
// ProjectTest.cpp - FinalTest of the Project                              //
// Author: Xiaohan Wang                                                    //
/////////////////////////////////////////////////////////////////////////////
#ifdef N_TEST_PROJECT
#include "../Displayer/Displayer.h"
#include <direct.h>
#include "../Filter/Filter.h"
#include "../Convert/Convert.h"
#include <iostream>
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"
using namespace FileSystem;
using namespace Utilities;
// -----< function getFilename to get the real name from the path >---------------
std::string getFilename(std::string target) {
	int pos = target.find_last_of('\\');
	std::string s(target.substr(pos + 1));
	return s;
}



int main(int argc, char *argv[])
{

	Title("Project2" );
	ProcessCmdLine pcl(argc, argv);
	preface("Command Line: ");
	pcl.showCmdLine();
	putline();
	cout << "Requirement 3:/nRight now the command line is input is "<< argv[1] << endl;
	DirExplorerN de(pcl.path());
	de.addPattern("*.cpp");
	de.addPattern("*.h");

	if (pcl.hasOption('s'))
	{
		de.recurse();
	}
	std::cout << "\nSeaching for files...\n";
	std::vector<std::string> filenames = de.getFiles();
	std::cout << "The following files are founded in the folder: " << endl;
	for (std::string m : filenames) {
		std::cout << m << "\n";
	}
	
	if (de.fileCount() == 0) {
		std::cout << "Didn't find any files, please check again";
	}
	de.showStats();
	Filter filter(argv[3]);
	std::cout << "\nFiltering using regex match...\n";
	std::vector<std::string> matches = filter.RegIt(filenames);
	if (matches.empty())
	{
		std::cout << "\nNo files matches by regex...\n Program stops.\n"; exit(1);
	}
	std::cout << "\nRequirement 5: Files have been loaded successfully"<<endl;
	std::cout << "The following files matches the regex: " << endl;
	for (std::string m : matches) {
		std::cout << m << "\n";
	}
	cout << "Ready to convert files to html..." << endl;
	for (std::string i : matches) Convert::FinialConvert(i);
	std::cout << "\nDisplaying\n";
	Displayer displayer("../convertedPages");
	displayer.DisplayFiles();


	return 0;
}
#endif // TEST_PUB