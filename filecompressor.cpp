#include "filecompressor.h"
#include <iostream>

void FreeCharNode(CharNode *node)
{
    // 递归清理节点
    for (auto it = node->Children.begin(); it != node->Children.end(); it++)
    {
        FreeCharNode(it->second);
        delete it->second;
    }
    node->Children.clear();
}

FileCompressor::FileCompressor(std::string rootDirectory, FileReader *packerDataFile)
{
    if (packerDataFile == nullptr)
    {
        Status = ERROR;
        return ;
    }

    PackerDataFile = packerDataFile;
    RootDirectory = rootDirectory;
    Counter = 0;
    Dictionary.CurrentIndex = 0;
    Status = COMPRESS;
    return ;
}

FileCompressor::FileCompressor(std::string rootDirectory)
{
    RootDirectory = rootDirectory;
    Counter = 0;
    Dictionary.CurrentIndex = 0;
    Status = DECOMPRESS;
    return ;
}

FileCompressor::~FileCompressor()
{
    CompressorFile.close();
    FreeCharNode(&Dictionary);
}

StatusCode FileCompressor::Compress()
{
    if (Status != COMPRESS)
        return ERROR_UNKNOW;

    CompressorFile.open(RootDirectory + COMPRESSOR_FILE_NAME, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!CompressorFile.is_open())
    {
        Status = ERROR;
        return ERROR_FILE_CANT_OPEN;
    }

    // 配置进度条
    off_t ProgressBarTotal{};    // 进图条总长度
    off_t ProgressBarCurrent{0}; // 进图条当前长度
    ProgressBarTotal = PackerDataFile->Length();

    CharNode *pNode = &Dictionary; //字典树当前节点
    char tmp{};

    // 采用LZ78算法进行压缩
    PackerDataFile->seekg(0);
    while (PackerDataFile->peek() != EOF)
    {
        // 读取字符并在当前节点下查找字符
        PackerDataFile->read((char *)&tmp, 1);
        std::unordered_map<char, CharNode *>::iterator it = pNode->Children.find(tmp);
        // 如果当前节点没有找到该字符，则添加该字符到当前节点，并重新回到头节点
        if (it == pNode->Children.end())
        {
            CharNode *tmpNode = new CharNode;
            tmpNode->CurrentIndex = ++Counter;
            if (Counter == 0)
            {
                std::cerr << "Out of Counter Range" << std::endl;
            }
            pNode->Children.insert(std::unordered_map<char, CharNode *>::value_type(tmp, tmpNode));

            // 写入压缩文件
            char *buff = new char[sizeof(unsigned int) + 1];
            *(unsigned int *)buff = pNode->CurrentIndex;
            buff[sizeof(unsigned int)] = tmp;
            CompressorFile.write(buff, sizeof(unsigned int) + 1);
            delete[] buff;

            pNode = &Dictionary;
        }
        // 如果找到该字符，则继续下一个字符的查找
        else
        {
            pNode = it->second;
        }

        // 更新进度条
        char adjust = (ProgressBarCurrent >> 20) & 0x1;
        ProgressBarCurrent += 1;
        if ((ProgressBarCurrent >> 20) & 0x1 == adjust + 1)
        {
            std::cout << "\rCompressing: (" << ProgressBarCurrent << " / " << ProgressBarTotal << ")" << std::flush;
        }
    }

    // 当最后一个字符读取结束后，需判断当前节点是不是根节点
    bool endWithNull{};
    // 若不是，则表明文件结尾最后一段在字典中，直接将编码加入压缩后文件
    if (pNode != &Dictionary)
    {
        char *buff = new char[sizeof(unsigned int) + 1];
        *(unsigned int *)buff = pNode->CurrentIndex;
        buff[sizeof(unsigned int)] = '\0';
        CompressorFile.write(buff, sizeof(unsigned int) + 1);
        delete[] buff;
        endWithNull = true;
    }
    else
    {
        endWithNull = false;
    }
    // 写入字典总节点数和后继字符标识
    CompressorFile.write((char *)&Counter, sizeof(unsigned int));
    CompressorFile.write((char *)&endWithNull, sizeof(bool));

    // 更新进度条
    std::cout << "\rCompressing: (" << ProgressBarTotal << " / " << ProgressBarTotal << ")" << std::endl;
    std::cout << "Dictionary Index num: " << Counter << std::endl;
    std::cout << "Compress File Size: " << CompressorFile.tellp() << std::endl;

    FreeCharNode(&Dictionary);
    CompressorFile.close();

    return NO_ERROR;
}

void FileCompressor::DeleteFile()
{
    remove((RootDirectory + COMPRESSOR_FILE_NAME).c_str());
    return ;
}

StatusCode FileCompressor::Decompress()
{
    if (Status != DECOMPRESS)
        return ERROR_UNKNOW;

    // 打开压缩文件
    std::cout << "Opening file: " << RootDirectory + COMPRESSOR_FILE_NAME << std::endl;
    CompressorFile.open(RootDirectory + COMPRESSOR_FILE_NAME, std::ios::in | std::ios::binary);
    if(!CompressorFile.is_open())
    {
        CompressorFile.close();
        printf("file open fail 1!\n");
        return ERROR_FILE_CANT_OPEN;
    }
    // 打开被压缩的文件
    std::fstream BackupFile = std::fstream(RootDirectory + PACKED_FILE_NAME, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!BackupFile.is_open())
    {
        BackupFile.close();
        printf("file open fail 2!\n");
        return ERROR_FILE_CANT_OPEN;
    }

    off_t TotalLength{}; // 压缩文件长度（文件末尾的标志信息不计入）
    off_t readLength{0}; // 已读取长度
    bool endWithNull{};  // 后继字符标识

    CompressorFile.seekg(0, SEEK_BASE_END);
    std::cout << "Compress File Size: " << CompressorFile.tellg() << std::endl;
    CompressorFile.seekg(-sizeof(bool) - sizeof(unsigned int), SEEK_BASE_END);
    CompressorFile.read((char *)&Counter, sizeof(unsigned int));
    CompressorFile.read((char *)&endWithNull, sizeof(bool));
    CompressorFile.seekg(-sizeof(bool) - sizeof(unsigned int), SEEK_BASE_END);
    std::cout << "Index Length: " << Counter << std::endl;

    // 获取压缩文件长度
    TotalLength = CompressorFile.tellg();

    // 如果压缩文件长度不是编码序列的整数倍则表明压缩文件损坏
    if (TotalLength % (sizeof(unsigned int) + 1) != 0)
        return ERROR_FILE_INFO_NOT_MATCH;

    // 截断压缩文件，不保留末尾标志信息
    CompressorFile.close();
#if __cplusplus >= 201703L
    std::filesystem::resize_file(RootDirectory + COMPRESSOR_FILE_NAME, TotalLength);
#else
    truncate((RootDirectory + COMPRESSOR_FILE_NAME).c_str(), TotalLength);
#endif

    CompressorFile.open(RootDirectory + COMPRESSOR_FILE_NAME, std::ios::in | std::ios::binary);

    char *buff = new char[sizeof(unsigned int) + 1]; // 压缩编码缓冲区
    std::map<unsigned int, std::string> Index2Node; // 初始化字典
    Index2Node.insert(std::map<unsigned int, std::string>::value_type(0, std::string("")));
    Counter = 0;

    // 解压缩算法
    CompressorFile.seekg(0);
    while (CompressorFile.peek() != EOF)
    {
        char adjust = (readLength >> 20) & 0x1;

        // 读取一个压缩编码
        CompressorFile.read(buff, sizeof(unsigned int) + 1);
        readLength += CompressorFile.gcount();
        unsigned int index = *(unsigned int *)buff;
        char tmp = buff[sizeof(unsigned int)];

        // 如果到达压缩文件末尾，则根据后继字符标识进行分类
        if (readLength == TotalLength)
        {
            unsigned int index = *(unsigned int *)buff;
            char tmp = buff[sizeof(unsigned int)];
            std::string &CurrentString = Index2Node.find(index)->second;
            if (!endWithNull)
                CurrentString += tmp;
            BackupFile.write(CurrentString.c_str(), CurrentString.size());
        }
        else
        {
            unsigned int index = *(unsigned int *)buff;
            char tmp = buff[sizeof(unsigned int)];
            std::string CurrentString = Index2Node.find(index)->second;
            CurrentString += tmp;
            Index2Node.insert(std::map<unsigned int, std::string>::value_type(++Counter, CurrentString));
            BackupFile.write(CurrentString.c_str(), CurrentString.size());
        }

        // 更新进度条
        if ((readLength >> 20) & 0x1 == adjust + 1)
        {
            std::cout << "\rDecompressing: (" << readLength << " / " << TotalLength << ")" << std::flush;
        }
    }

    // 更新进度条
    std::cout << "\rDecompressing: (" << TotalLength << " / " << TotalLength << ")" << std::endl;
    std::cout << "Dictionary Index num: " << Counter << std::endl;

    delete[] buff;
    Index2Node.clear();
    BackupFile.close();
    CompressorFile.close();

    return NO_ERROR;
}
