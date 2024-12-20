#ifndef FILEREADER_H
#define FILEREADER_H

#include <fstream>
#include <filesystem>
#include <string>


class FileReader : public std::fstream
{
private:
    std::string FilePath; // 记录文件路径

public:
    //打开文件
    // truncateFile: 是否清空文件
    FileReader(const std::string &filePath, bool truncateFile);
    ~FileReader();
    // 返回文件长度
    size_t Length();
    // 返回文件所在路径
    std::string Path();
};
#endif // FILEREADER_H
