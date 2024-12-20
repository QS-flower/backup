#include "fileinformation.h"

FileInformation::FileInformation(const std::string &relativePath, FilePacker *packer, const std::string &rootDirectory)
{
    Packer = packer;
    RootDirectory = rootDirectory;
    if (RootDirectory.back() != '/')
        RootDirectory += '/';
    UnitInfo.RelativePath = relativePath;
    UnitInfo.rootPath = rootDirectory + relativePath;
    // 从文件本身获取元信息
    StatusCode state = GetUnitDataForBackup();
    if (state != NO_ERROR)
    {
        Status = ERROR;
        return;
    }
    Status = GOT_INFO;
    return;
}

FileInformation::FileInformation(FilePacker *packer, const std::string &rootDirectory)
{
    Packer = packer;
    RootDirectory = rootDirectory;
    if (RootDirectory.back() != '/')
        RootDirectory += '/';
    // 从备份文件获取元信息
    StatusCode state = GetUnitDataForRestore();
    if (state != NO_ERROR)
    {
        Status = ERROR;
        return;
    }
    Status = GOT_INFO;
    return;
}

FileInformation::~FileInformation() = default;

StatusCode FileInformation::GetUnitDataForBackup()
{
    struct stat tmp_stat{};

    if (Status == ERROR)
        return ERROR_UNKNOW;

    // if file or directory not exists
    if (lstat((RootDirectory + UnitInfo.RelativePath).c_str(), &tmp_stat) == -1)
    //lstat为软链接，c_str返回指向c的指针常量，内容与string相同，获取源文件信息
    {
        Status = ERROR;
        return ERROR_NOT_EXISTS;
    }

    // else, file or directory exists
    else
    {
        // 不同类型文件不同的部分
        switch (tmp_stat.st_mode & S_IFMT)
        {
        case S_IFREG://普通文件
            UnitInfo.Type = FILE_TYPE_FILE;
            UnitInfo.Size = tmp_stat.st_size;
            break;
        case S_IFLNK://符号链接
            UnitInfo.Type = FILE_TYPE_SYMLINK;
            UnitInfo.Size = tmp_stat.st_size;
            break;
        case S_IFIFO://FIFO（管道）文件
            UnitInfo.Type = FILE_TYPE_FIFO;
            printf("fifo\n");
            break;
        case S_IFDIR://目录
            UnitInfo.Type = FILE_TYPE_DIRECTORY;
            break;
        default:
            Status = ERROR;
            return ERROR_FILE_TYPE_UNKNOWN;
        }
        // 不同类型文件相同的部分
        UnitInfo.Inode = tmp_stat.st_ino;
        UnitInfo.InodeCount = tmp_stat.st_nlink;
        UnitInfo.Auth = tmp_stat.st_mode & 07777;
        UnitInfo.Atime = tmp_stat.st_atim;
        UnitInfo.UserID = tmp_stat.st_uid;
        UnitInfo.GroupID = tmp_stat.st_gid;
        UnitInfo.Size = tmp_stat.st_size;

        // 将inode信息加入维护，可以针对硬链接减少额外保存数据
        // if inode exists in inodeRecorder, get backupBegin
        if (InodeRecorder::inodeRecorderBackup->IsInodeExists(UnitInfo.Inode))
        {
            InodeRecorder::inodeRecorderBackup->GetInodeOffset(UnitInfo.Inode, UnitInfo.StartOffset);
        }
        // else get the backupBegin and insert into inodeRecorder
        else
        {
            UnitInfo.StartOffset = Packer->DataFile->Length();
            InodeRecorder::inodeRecorderBackup->AddInode(UnitInfo.Inode, UnitInfo.StartOffset);
        }

        // 计算元信息结构体总长
        UnitInfo.TotalLength = FILE_UNIT_INFO_HEAD_SIZE + UnitInfo.RelativePath.size()+UnitInfo.rootPath.size();
        Status = GOT_INFO;
        return NO_ERROR;
    }
}

StatusCode FileInformation::GetUnitDataForRestore()
{
    if (Status == ERROR)
        return ERROR_UNKNOW;

    off_t oneUnitSize{}; // 记录一个元信息结构体的长度
    size_t readLength{}; // 已读长度

    // 获取一个元信息
    // 先获取元信息结构体的长度，再读取其中的内容
    Packer->UnitFile->read((char *)&oneUnitSize, sizeof(off_t));
    if (Packer->UnitFile->gcount() != sizeof(off_t))
    {
        Status = ERROR;
        return ERROR_FILE_CANT_READ;
    }
    Packer->UnitFile->seekg(-sizeof(off_t), SEEK_BASE_CUR);
    char *buffer = new char[oneUnitSize];
    Packer->UnitFile->read(buffer, oneUnitSize);
    if (Packer->UnitFile->gcount() != oneUnitSize)
    {
        Status = ERROR;
        delete[] buffer;
        buffer = nullptr;
        return ERROR_FILE_CANT_READ;
    }
    UnitInfo.TotalLength = ((FileUnitInfo *)buffer)->TotalLength;
    UnitInfo.Inode = ((FileUnitInfo *)buffer)->Inode;
    UnitInfo.InodeCount = ((FileUnitInfo *)buffer)->InodeCount;
    UnitInfo.Type = ((FileUnitInfo *)buffer)->Type;
    UnitInfo.Auth = ((FileUnitInfo *)buffer)->Auth;
    UnitInfo.Atime = ((FileUnitInfo *)buffer)->Atime;
    UnitInfo.UserID = ((FileUnitInfo *)buffer)->UserID;
    UnitInfo.GroupID = ((FileUnitInfo *)buffer)->GroupID;
    UnitInfo.Size = ((FileUnitInfo *)buffer)->Size;
    UnitInfo.StartOffset = ((FileUnitInfo *)buffer)->StartOffset;
    UnitInfo.RelativePath = std::string(buffer + FILE_UNIT_INFO_HEAD_SIZE, oneUnitSize - FILE_UNIT_INFO_HEAD_SIZE);
    int a = UnitInfo.RelativePath.find("/home");
    UnitInfo.RelativePath = UnitInfo.RelativePath.substr(0,a);
    UnitInfo.rootPath = std::string(buffer + FILE_UNIT_INFO_HEAD_SIZE + UnitInfo.RelativePath.size(), oneUnitSize - FILE_UNIT_INFO_HEAD_SIZE - UnitInfo.RelativePath.size());
    delete[] buffer;
    buffer = nullptr;

    // 将inode对应的路径加入维护，用于创建硬链接
    // if inode not exists in inodeRecorder, insert relative path into inodeRecorder
    if (!InodeRecorder::inodeRecorderRestore->IsInodeExists(UnitInfo.Inode))
    {
        InodeRecorder::inodeRecorderRestore->AddInode(UnitInfo.Inode, UnitInfo.rootPath);
    }

    Status = GOT_INFO;
    return NO_ERROR;
}

StatusCode FileInformation::Backup()
{
    if (Status != GOT_INFO)
        return ERROR_UNKNOW;

    // 将UnitFile写指针移动到备份文件的末尾，添加新的元信息数据。
    // 并记录起始偏移到DirectoryUnitOffset中，以便验证时使用
    Packer->UnitFile->seekp(0, SEEK_BASE_END);//指定下一个进入缓冲区的字符所在的位置
    DirectoryUnitOffset = Packer->UnitFile->tellp();//获取当前输出流最后一个字符所在的位置
    Packer->UnitFile->write((char *)&UnitInfo, FILE_UNIT_INFO_HEAD_SIZE);
    Packer->UnitFile->write(UnitInfo.RelativePath.c_str(), UnitInfo.RelativePath.size());
    Packer->UnitFile->write(UnitInfo.rootPath.c_str(), UnitInfo.rootPath.size());

    // 获取备份文件长度，用于判断文件数据是否需要备份，（不是第一次出现的同一个inode的硬链接不用备份）
    off_t backupLength = Packer->DataFile->Length();
    switch (UnitInfo.Type)
    {
    case FILE_TYPE_FILE:
        // 如果备份起始偏移是DataFile的长度，说明需要将数据添加到DataFile尾部
        // 如果偏移不是DataFile的长度，说明数据已经在DataFile中，不用再次备份
        if (UnitInfo.StartOffset == backupLength)
        {
            char *buffer = new char[BUFF_LENGTH];
            Packer->DataFile->seekp(0, SEEK_BASE_END);
            FileStream.open((RootDirectory + UnitInfo.RelativePath).c_str(), std::ios::in | std::ios::binary);
            while (FileStream.peek() != EOF)
            {
                FileStream.read(buffer, BUFF_LENGTH);
                off_t tmpReadLength = FileStream.gcount();
                Packer->DataFile->write(buffer, tmpReadLength);
            }
            FileStream.close();
            delete[] buffer;
            buffer = nullptr;
        }
        break;
    case FILE_TYPE_FIFO:
        // FIFO文件没有数据，不需要备份
        break;
    case FILE_TYPE_SYMLINK:
        // 如果备份起始偏移是DataFile的长度，说明需要将数据添加到DataFile尾部
        // 如果偏移不是DataFile的长度，说明数据已经在DataFile中，不用再次备份
        if (UnitInfo.StartOffset == backupLength)
        {
            char *buffer = new char[UnitInfo.Size];
            readlink((RootDirectory + UnitInfo.RelativePath).c_str(), buffer, UnitInfo.Size);
            Packer->DataFile->seekp(0, SEEK_BASE_END);
            Packer->DataFile->write(buffer, UnitInfo.Size);
            delete[] buffer;
            buffer = nullptr;
        }
        break;
    case FILE_TYPE_DIRECTORY:
    {
        // 目录本身没有数据需要备份到DataFile，但需要完成对所有需要备份的目录项的遍历和备份才算备份完成，才可验证本目录的备份
        DIR *openedDir = opendir((RootDirectory + UnitInfo.RelativePath).c_str());
        dirent *dirEntry{};
        while ((dirEntry = readdir(openedDir)) != nullptr)
        {
            // 忽略.和..
            if (std::string(dirEntry->d_name) == "." || std::string(dirEntry->d_name) == "..")
                continue;
            // 只备份FIFO、文件、符号链接和目录
            switch (dirEntry->d_type)
            {
            case DT_FIFO:
            case DT_LNK:
            case DT_REG:
            case DT_DIR:
            {
                // 对每一个需要备份的目录项发起备份
                std::string fileName = UnitInfo.RelativePath + "/" + std::string(dirEntry->d_name);
                FileInformation *fileInfo = new FileInformation(fileName, Packer, RootDirectory);
                if (!fileInfo->good())
                {
                    Status = ERROR;
                    delete fileInfo;
                    fileInfo = nullptr;
                    return ERROR_UNKNOW;
                }
                if (fileInfo->Backup() != NO_ERROR)
                {
                    Status = ERROR;
                    delete fileInfo;
                    fileInfo = nullptr;
                    return ERROR_UNKNOW;
                }
                delete fileInfo;
                fileInfo = nullptr;

                // 将已备份的目录项inode记录，用于本目录的备份验证
                DirectoryChildrenInodesQueue.push(dirEntry->d_ino);
            }
            break;
            default:
                break;
            }
        }
        break;
    }
    default:
        Status = ERROR;
        return ERROR_FILE_TYPE_UNKNOWN;
    }

    // 备份验证
    return NO_ERROR;
}

StatusCode FileInformation::Restore()
{
    if (Status != GOT_INFO)
        return ERROR_UNKNOW;

    std::string savedPath{}; // 记录已经维护的inode对应的路径
    InodeRecorder::inodeRecorderRestore->GetInodePath(UnitInfo.Inode, savedPath);
    // 如果已经维护过，则建立硬链接即可
    if (savedPath != UnitInfo.rootPath || (UnitInfo.InodeCount > 1 && !FILE_TYPE_DIRECTORY))
    {
        printf("link,%d\n",UnitInfo.InodeCount);
        link(UnitInfo.rootPath.c_str(), (RootDirectory + UnitInfo.RelativePath).c_str());
    }
    // 如果没有维护过，则正常还原文件数据
    else
    {
        switch (UnitInfo.Type)
        {
        case FILE_TYPE_FILE:
        {
            FileStream.open((RootDirectory + UnitInfo.RelativePath).c_str(), std::ios::out | std::ios::binary);
            char *buffer = new char[BUFF_LENGTH];
            size_t readLength{};
            off_t leftBytes = UnitInfo.Size;
            Packer->DataFile->seekg(UnitInfo.StartOffset);
            while (leftBytes > 0)
            {
                if (BUFF_LENGTH < leftBytes)
                    Packer->DataFile->read(buffer, BUFF_LENGTH);
                else
                    Packer->DataFile->read(buffer, leftBytes);
                readLength = Packer->DataFile->gcount();
                FileStream.write(buffer, readLength);
                leftBytes -= readLength;
            }
            delete[] buffer;
            buffer = nullptr;
            FileStream.close();
        }
        break;
        case FILE_TYPE_FIFO:
        {
            printf("fifo:%d %s\n",UnitInfo.Type,(RootDirectory + UnitInfo.RelativePath).c_str());
            mkfifo((RootDirectory + UnitInfo.RelativePath).c_str(), UnitInfo.Auth);
        }
        break;
        case FILE_TYPE_SYMLINK:
        {
            printf("symlink:%d %s\n",UnitInfo.Type,(RootDirectory + UnitInfo.RelativePath).c_str());
            size_t readLength{};
            char *buffer = new char[UnitInfo.Size];
            Packer->DataFile->seekg(UnitInfo.StartOffset);
            Packer->DataFile->read(buffer, UnitInfo.Size);
            std::string destination = std::string(buffer, UnitInfo.Size);
            symlink(UnitInfo.rootPath.c_str(), (RootDirectory + UnitInfo.RelativePath).c_str());
            delete[] buffer;
        }
        break;
        case FILE_TYPE_DIRECTORY:
        {
            printf("directory:%d %s\n",UnitInfo.Type,(RootDirectory + UnitInfo.RelativePath).c_str());
            if(int a = mkdir((RootDirectory + UnitInfo.RelativePath).c_str(), UnitInfo.Auth) == -1)
            {
                printf("mkdir error\n");
            }
        }
        break;
        default:
            Status = ERROR;
            return ERROR_FILE_TYPE_UNKNOWN;
            break;
        }
    }

    // 获取备份的时间戳，并还原
    utimbuf utb{};
    utb.actime = UnitInfo.Atime.tv_sec;
    utb.modtime = UnitInfo.Atime.tv_sec;
    utime((RootDirectory + UnitInfo.RelativePath).c_str(), &utb);

    // 还原备份的权限和属主
    chmod((RootDirectory + UnitInfo.RelativePath).c_str(), UnitInfo.Auth);
    lchown((RootDirectory + UnitInfo.RelativePath).c_str(), UnitInfo.UserID, UnitInfo.GroupID);

    Status = RESTORED;
    return NO_ERROR;
}

bool FileInformation::good()
{
    return Status != ERROR;
}
