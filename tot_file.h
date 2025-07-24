#ifndef TOT_FILE_H
#define TOT_FILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
namespace tot{
std::string replacePlaceholder(std::string& input,std::string& placeholder,std::string& replacement);
std::string resetTemplate(std::string filePath);
void createFile(std::string filePath,std::map<std::string,std::string> replaceMap,bool overwrite,std::string& content);
std::string underscoreToUpperCamelCase(const std::string& input);
std::string underscoreToCamelCaseFirstLower(const std::string& input);
}

#endif // TOT_FILE_H
