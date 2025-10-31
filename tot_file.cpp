#include "tot_file.h"
#include <filesystem>
namespace  tot{

// 简单字符串替换函数：替换所有 `${1}` 为 abc
std::string replacePlaceholder(std::string& input,const std::string& placeholder,const std::string& replacement) {
    std::string result = input;
    size_t pos = 0;
    while ((pos = result.find(placeholder, pos)) != std::string::npos) {
        result.replace(pos, placeholder.length(), replacement);
        pos += replacement.length();
    }
    return result;
}

std::string resetTemplate(std::string filePath){
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    return content;
}
void createFile(std::string filePath,std::map<std::string,std::string> replaceMap,bool overwrite,std::string& content){
    // 替换filepath
    for(auto& pair:replaceMap){
        filePath = replacePlaceholder(filePath, pair.first, pair.second);
    }
    if (std::filesystem::exists(filePath) && !overwrite){
        return;
    }
    std::filesystem::path path(filePath);

    // 创建目录（包括多级）
    std::filesystem::create_directories(path.parent_path());
    std::ofstream file(filePath);  // 创建或覆盖文件
    file <<content;
    file.close();
}

std::string underscoreToUpperCamelCase(const std::string& input) {
    std::string pascal_case_str;
    // 初始为 true，确保字符串的第一个字符（如果不是下划线）被大写
    bool capitalize_next = true;
    for (char c : input) {
        if (c == '_') {
            // 遇到下划线，设置标记，下一个非下划线字符需要大写
            capitalize_next = true;
        } else {
            if (capitalize_next) {
                // 如果需要大写，则将其转换为大写字母
                pascal_case_str += static_cast<char>(std::toupper(c));
                capitalize_next = false; // 大写后重置标记
            } else {
                // 否则直接添加字符（保持原有大小写）
                pascal_case_str += c;
            }
        }
    }
    return pascal_case_str;
}

std::string underscoreToCamelCaseFirstLower(const std::string& input) {
    std::string result;
    bool toUpper = false;

    for (char ch : input) {
        if (ch == '_') {
            toUpper = true;
        } else {
            if (toUpper) {
                // 转大写
                if (ch >= 'a' && ch <= 'z') {
                    result += ch - 'a' + 'A';
                } else {
                    result += ch;
                }
                toUpper = false;
            } else {
                result += ch;
            }
        }
    }

    // 保证首字母小写
    if (!result.empty() && result[0] >= 'A' && result[0] <= 'Z') {
        result[0] = result[0] - 'A' + 'a';
    }

    return result;
}
std::string getFirstLetters(const std::string& input) {
  std::string result;
  std::istringstream iss(input);
  std::string word;

  // 使用 getline 分割 '_'
  while (std::getline(iss, word, '_')) {
    if (!word.empty()) {
      result += word[0];  // 取每个单词的首字母
    }
  }

  return result;
}

}
