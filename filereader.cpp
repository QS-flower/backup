#include "filereader.h"

FileReader::FileReader(const std::string &filePath, bool truncateFile)
{
    FilePath = filePath;
    if (truncateFile)
        open(FilePath, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    else
        open(FilePath, std::ios::in | std::ios::out | std::ios::binary);
}

FileReader::~FileReader() = default;

size_t FileReader::Length()
{
    if (is_open())
    {
        // 通过将读指针定位到文件末尾以获得文件长度
        auto readPointer = tellg();
        seekg(0, std::ios::end);
        size_t length = tellg();
        seekg(readPointer);
        return length;
    }

    return 0;
}

std::string FileReader::Path()
{
    if (is_open())
        return FilePath;
    else
        return std::string("");
}
