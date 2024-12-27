#include "filepacker.h"
#include "filereader.h"

FilePacker::FilePacker(const std::string &directoryPath, bool ForBackup)
{
    DirectoryPath = directoryPath;

    if (ForBackup)
    {
        // 打包时，先将数据和元数据分别存为两个文件，故状态设置为SEPARATE_OPENED
        Status = SEPARATE_OPENED;
        DataFile = new FileReader(directoryPath + DATA_FILE_NAME, true);//数据文件，DATA.BACKUP
        UnitFile = new FileReader(directoryPath + UNIT_FILE_NAME, true);//元数据文件，UNIT.BACKUP
        BackupFile = nullptr;
        if (!DataFile->is_open() || !UnitFile->is_open())//两文件都需打开
            Status = ERROR;
        off_t tmp0 = 0;
        // 根据文件格式协议，文件头存放一个sizeof(off_t)的数据，用于记录文件总长度，备份完成前用0填充
        DataFile->write((char *)&tmp0, sizeof(off_t));
        UnitFile->write((char *)&tmp0, sizeof(off_t));
    }
    // else
    // {
    //     // 还原时，数据先位于一个文件，需要拆成两个存放不同内容的文件，故状态设置为ALL_IN_ONE_OPENED
    //     Status = ALL_IN_ONE_OPENED;
    //     DataFile = nullptr;
    //     UnitFile = nullptr;
    //     BackupFile = new FileReader(directoryPath + PACKED_FILE_NAME, false);
    //     if (!BackupFile->is_open())
    //         Status = ERROR;
    // }
}

FilePacker::~FilePacker()
{
    if (DataFile != nullptr)
    {
        delete DataFile;
        DataFile = nullptr;
    }
    if (UnitFile != nullptr)
    {
        delete UnitFile;
        UnitFile = nullptr;
    }
    if (BackupFile != nullptr)
    {
        delete BackupFile;
        BackupFile = nullptr;
    }
}

StatusCode FilePacker::CompactFile()//整合元数据文件与数据文件
{
    if (Status != SEPARATE_OPENED)//验证元数据文件与数据文件是分别存储状态
        return ERROR_UNKNOW;

    // 文件读取缓冲区
    char *buffer = new char[BUFF_LENGTH];
    // 每次读取的实际读取长度
    size_t readLength;

    DataFile->seekp(0);
    UnitFile->seekp(0);

    // 将两个文件的长度写入文件头部
    off_t fileLength{};
    fileLength = DataFile->Length();
    DataFile->write((char *)&fileLength, sizeof(off_t));
    fileLength = UnitFile->Length();
    UnitFile->write((char *)&fileLength, sizeof(off_t));

    // 从UnitFile读取数据写到DataFile尾部，合并两文件
    DataFile->seekp(0, SEEK_BASE_END);
    UnitFile->seekg(0);
    DataFile->clear();
    UnitFile->clear();
    while (UnitFile->peek() != EOF)
    {
        UnitFile->read(buffer, BUFF_LENGTH);
        if (!DataFile->good())
        {
            delete[] buffer;
            return ERROR_FILE_CANT_WRITE;
        }
        readLength = UnitFile->gcount();
        DataFile->write(buffer, readLength);
        if (!DataFile->good())
        {
            delete[] buffer;
            return ERROR_FILE_CANT_WRITE;
        }
    }
    delete[] buffer;
    buffer = nullptr;
    delete DataFile;
    DataFile = nullptr;
    delete UnitFile;
    UnitFile = nullptr;

    // 将文件重命名为PACKED.BACKUP，并用BackupFile指向文件
    rename((DirectoryPath + DATA_FILE_NAME).c_str(), (DirectoryPath + PACKED_FILE_NAME).c_str());
    BackupFile = new FileReader(DirectoryPath + PACKED_FILE_NAME, false);//返回状态码
    Status = COMPACTED;//状态设为已打包
    return NO_ERROR;
}

/*以下为解包*/
// StatusCode FilePacker::Disassemble()
// {
//     if (Status != ALL_IN_ONE_OPENED)
//         return ERROR_UNKNOW;

//     // 文件读取缓冲区
//     char *buffer = new char[BUFF_LENGTH];
//     // 实际读取长度
//     size_t readLength{};
//     // UnitFile在整合的文件中的偏移量
//     off_t unitOffset{};

//     // 通过文件头部的文件长度信息获取DataFile的长度，其后就是UnitFile了
//     BackupFile->seekg(0);
//     BackupFile->read((char *)&unitOffset, sizeof(unitOffset));
//     if (!BackupFile->good() || BackupFile->gcount() != sizeof(unitOffset))
//     {
//         delete[] buffer;
//         return ERROR_FILE_CANT_READ;
//     }

//     // 从整合的文件中读取UnitFile的内容并单独存储
//     BackupFile->seekg(unitOffset);
//     UnitFile = new FileReader(DirectoryPath + UNIT_FILE_NAME, true);
//     UnitFile->seekp(0);
//     while (BackupFile->peek() != EOF)
//     {
//         BackupFile->read(buffer, BUFF_LENGTH);
//         readLength = BackupFile->gcount();
//         UnitFile->write(buffer, readLength);
//         if (!UnitFile->good())
//         {
//             delete[] buffer;
//             return ERROR_FILE_CANT_WRITE;
//         }
//     }
//     delete[] buffer;
//     buffer = nullptr;
//     delete BackupFile;
//     BackupFile = nullptr;

//     // 将文件重命名为DATA.BACKUP，并用DataFile指向文件，并将UnitFile的内容删除
//     rename((DirectoryPath + PACKED_FILE_NAME).c_str(), (DirectoryPath + DATA_FILE_NAME).c_str());
// #if __cplusplus >= 201703L
//     std::filesystem::resize_file(DirectoryPath + DATA_FILE_NAME, unitOffset);
// #else
//     truncate((DirectoryPath + DATA_FILE_NAME).c_str(), unitOffset);
// #endif
//     DataFile = new FileReader(DirectoryPath + DATA_FILE_NAME, false);
//     UnitFile->seekg(0);
//     UnitFile->seekp(0);

//     // 对文件长度记录和实际长度进行校验
//     off_t fileLength{};
//     DataFile->read((char *)&fileLength, sizeof(off_t));
//     if (fileLength != DataFile->Length())
//     {
//         Status = ERROR;
//         return ERROR_FILE_INFO_NOT_MATCH;
//     }
//     UnitFile->read((char *)&fileLength, sizeof(off_t));
//     if (fileLength != UnitFile->Length())
//     {
//         Status = ERROR;
//         return ERROR_FILE_INFO_NOT_MATCH;
//     }
//     Status = DISASSEMBLED;
//     return NO_ERROR;
// }

std::string FilePacker::Directory_Path()
{
    if (Status == ERROR)
        return std::string("");
    else
        return DirectoryPath;
}

bool FilePacker::good()
{
    return Status != ERROR;
}

void FilePacker::DeleteFile()
{
    // remove((DirectoryPath + DATA_FILE_NAME).c_str());
    remove((DirectoryPath + UNIT_FILE_NAME).c_str());
    remove((DirectoryPath + PACKED_FILE_NAME).c_str());
}
