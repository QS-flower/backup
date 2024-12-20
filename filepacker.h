#ifndef FILEPACKER_H
#define FILEPACKER_H
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdlib>
#include <QWidget>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <iostream>
#include <QApplication>
#include <QProcess>

#include "state.h"
#include "filereader.h"

#if __cplusplus >= 201703L
#include <filesystem>
#else
#include <unistd.h>
#endif

#define BUFF_LENGTH (4096 * 1024) // 缓冲区长度

#define SEEK_BASE_BEG std::ios::beg // 定位到文件开头
#define SEEK_BASE_CUR std::ios::cur // 定位到当前位置
#define SEEK_BASE_END std::ios::end // 定位到文件结尾

#define DATA_FILE_NAME "DATA.BACKUP"     // 数据文件名
#define UNIT_FILE_NAME "UNIT.BACKUP"     // 元数据文件名
#define PACKED_FILE_NAME "BACKUP.BACKUP" // 打包文件名
class FilePacker
{
    private:
        std::string DirectoryPath; // 记录备份文件所在的目录
        enum
        {
            SEPARATE_OPENED,   // 独立文件已打开
            COMPACTED,         // 已整合
            ALL_IN_ONE_OPENED, //整合文件已打开
            DISASSEMBLED,      //已拆分
            ERROR              // 错误
        } Status{};            // 记录状态

    public:
        FileReader *DataFile;   // 用于记录文件实际数据的文件
        FileReader *UnitFile;   // 用于记录文件元信息的文件
        FileReader *BackupFile; // 指向打包后文件的指针
        // 构造函数，记录目录路径，并打开文件
        // ForBackup: true表示用于备份，false表示还原
        FilePacker(const std::string &directoryPath, bool ForBackup);
        ~FilePacker();
        // 将文件整合
        StatusCode CompactFile();
        // 将文件拆解
        /*StatusCode Disassemble();*/
        // 获取文件夹路径
        std::string Directory_Path();
        // 判断状态
        bool good();
        // 删除维护的文件和目录
        void DeleteFile();
};

#endif // FILEPACKER_H
