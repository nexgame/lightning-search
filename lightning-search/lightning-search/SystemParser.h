#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <codecvt>
#include <regex>
#include <map>
#include <list>
#include <set>
#include <curl/curl.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/algorithm/string.hpp>


#ifndef SystemParser_h
#define SystemParser_h

class SystemParser {
public:
	// SystemParser();
	void run(void);
	void setScanDone(bool done);
	bool getScanDone(void);
	void FindFile(const std::wstring& directory);
	void indexFiles();
private:
	void checkFile(std::wstring filename, std::wstring directory, WIN32_FIND_DATA file);
};

#endif
