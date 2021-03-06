/////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages  //
// ver 1.0                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2019           //
/////////////////////////////////////////////////////////////////////////

#include "ServerPrototype.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include <chrono>
#include "../ProjectTest/CodePublisher.h"
namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

Files Server::getCodes(const Repository::SearchPath& path)
{
 CodePublisher* cp = new CodePublisher();
	Files f= cp->getCodes(path);
  return f;
}
Files Server::getFiles(const Repository::SearchPath& path)
{
	CodePublisher* cp = new CodePublisher();
	Files f = cp->getFiles(path);
	return f;
}

Dirs Server::getDirs(const Repository::SearchPath& path)
{
  return Directory::getDirectories(path);
}

template<typename T>
void show(const T& t, const std::string& msg)
{
  std::cout << "\n  " << msg.c_str();
  for (auto item : t)
  {
    std::cout << "\n    " << item.c_str();
  }
}

std::function<Msg(Msg)> echo = [](Msg msg) {
  Msg reply = msg;
  reply.to(msg.from());
  reply.from(msg.to());
  return reply;
};

std::function<Msg(Msg)> getFiles = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getFiles");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
    Files files = Server::getCodes(searchPath);
    size_t count = 0;
    for (auto item : files)
    {
      std::string countStr = Utilities::Converter<size_t>::toString(++count);
      reply.attribute("file" + countStr, item);
    }
  }
  else
  {
    std::cout << "\n  getFiles message did not define a path attribute";
  }
  return reply;
};

std::function<Msg(Msg)> getMatchedFiles = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getMatchedFiles");
	std::string path = msg.value("path");
	if (path != "")
	{
		std::string searchPath = storageRoot;
		if (path != ".")
			searchPath = searchPath + "\\" + path;
		Files filesPre = Server::getCodes(searchPath);
		CodePublisher *cp=new CodePublisher();
		string reg = msg.value("regEx");
		Files files=cp->getMatchedFileName(filesPre,reg);
		size_t count = 0;
		for (auto item : files)
		{
			std::string countStr = Utilities::Converter<size_t>::toString(++count);
			reply.attribute("file" + countStr, item);
		}
	}
	else
	{
		std::cout << "\n  getMatchedFiles message did not define a path attribute";
	}
	return reply;
};
std::function<Msg(Msg)> getPages = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getPages");

	std::string searchPath = "../convertedPages";
		Files pages = Server::getFiles(searchPath);
		size_t count = 0;
		for (auto item : pages)
		{
			std::string countStr = Utilities::Converter<size_t>::toString(++count);
			reply.attribute("page" + countStr, item);
		}
	

	return reply;
};

std::function<Msg(Msg)> getDirs = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getDirs");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
    Files dirs = Server::getDirs(searchPath);
    size_t count = 0;
    for (auto item : dirs)
    {
      if (item != ".." && item != ".")
      {
        std::string countStr = Utilities::Converter<size_t>::toString(++count);
        reply.attribute("dir" + countStr, item);
      }
    }
  }
  else
  {
    std::cout << "\n  getDirs message did not define a path attribute";
  }
  return reply;
};

std::function<Msg(Msg)> convertFile = [](Msg msg) {
	string filepath = msg.value("filepath");
	CodePublisher* cp=new CodePublisher();
	cp->ConvertFile(filepath);
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getPages");

	std::string searchPath = "../convertedPages";
	Files pages = Server::getFiles(searchPath);
	size_t count = 0;
	for (auto item : pages)
	{
		std::string countStr = Utilities::Converter<size_t>::toString(++count);
		reply.attribute("page" + countStr, item);
	}


	return reply;
};
//<---function to transfer all files matches parten into html5 format--->
std::function<Msg(Msg)> ConvertAll = [](Msg msg) {
	string filepath = msg.value("path");
	string reg = msg.value("reg");
	CodePublisher* cp = new CodePublisher();
	cp->ConvertUnderPath(filepath,reg);
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getPages");

	std::string searchPath = "../convertedPages";
	Files pages = Server::getFiles(searchPath);
	size_t count = 0;
	for (auto item : pages)
	{
		std::string countStr = Utilities::Converter<size_t>::toString(++count);
		reply.attribute("page" + countStr, item);
	}
	return reply;
};
std::function<Msg(Msg)> getCodes = [](Msg msg) {
	string filepath = msg.value("filepath");
	FileSystem::File f(filepath);
	string codes = f.readAll();
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getCodes");
	reply.attribute("code" , codes);
	return reply;
};
//<---Initialize the server side--->
void serverInitialize() {
	Server server(serverEndPoint, "ServerPrototype");
	server.start();

	std::cout << "\n  testing getFiles and getDirs methods";
	std::cout << "\n --------------------------------------";
	Files files = server.getFiles();
	show(files, "Files:");
	Dirs dirs = server.getDirs();
	show(dirs, "Dirs:");
	std::cout << "\n";

	std::cout << "\n  testing message processing";
	std::cout << "\n ----------------------------";
	server.addMsgProc("echo", echo);
	server.addMsgProc("getFiles", getFiles);
	server.addMsgProc("getMatchedFiles", getMatchedFiles);
	server.addMsgProc("convertFile", convertFile);
	server.addMsgProc("getPages", getPages);
	server.addMsgProc("getDirs", getDirs);
	server.addMsgProc("AskForCode", getCodes);
	server.addMsgProc("convertAll", ConvertAll);
	server.addMsgProc("serverQuit", echo);
	server.processMessages();

	Msg msg(serverEndPoint, serverEndPoint);  // send to self
	msg.name("msgToSelf");
	msg.command("echo");
	msg.attribute("verbose", "show me");
	server.postMessage(msg);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	msg.command("getFiles");
	msg.remove("verbose");
	msg.attributes()["path"] = storageRoot;
	server.postMessage(msg);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	msg.command("getDirs");
	msg.attributes()["path"] = storageRoot;
	server.postMessage(msg);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	std::cout << "\n  press enter to exit";
	std::cin.get();
	std::cout << "\n";

	msg.command("serverQuit");
	server.postMessage(msg);
	server.stop();
}
int main()
{
	std::cout << "\nAutomatic Test. will run the whole project with all requirement reaches!\n";
	std::cout << "\Introcution to the GUI：\n";
	std::cout << "\The top window is the nevigator window, it will shows the dictionaries in a folder. Double click to join a sub-root\n";
	std::cout << "\The middle window shows the cpp/h files in the current window. You can type in valid regular expression and use the buttons.\n The right button will show the matched file names in the current folder.\n the left button will scan the current dictionary and all its sub-dictionaries to find matched code files and convert them all to the display view. You can also double click a file to convert only one file.\n";
	std::cout << "\The third window is the display window, it will shows the html file in a code mode or in a browser mode. Double click to see!t\n";
	std::cout << "\n  Requirement 1-2: will run automatically and I will print running details to show how are the communications for each command between server and GUI.";
  std::cout << "\n ==============================";
  std::cout << "\nRequirement 3-4: To test requiremnt 3, just try click on the dictionaries on the top window so that you will find the interface is greatlty used to display files matched .cpp or .h";
  std::cout << "\nRequirement 6:You can see the GUI if you run the program.\n";
  std::cout << "\nRequirement 7:Try double-click on a file from the middle window to see how one file is converted to html file.";

  serverInitialize();

  return 0;
}

