#include "up_down.h"
#include <cstring>

char *getTime()
{
    time_t rawTime;
    tm *pstTime{};
    char *buf = new char[30];

    time(&rawTime);
    pstTime = localtime(&rawTime);

    strftime(buf,20,"%Y-%m-%d %H:%M:%S",pstTime);

    return buf;
}

int upload(const std::string &path, const QString &name, QString &errorMessage)
{
    int fp_r;
    int fp_w;
    long filesize{0};
    char *time_s;
    int count_r, count_w;

    struct stat statbuf;
    char filepath[100];
    char filepackage[100];

    // 打开源文件
    if ((fp_r = ::open(path.c_str(), O_RDONLY)) == -1)
    {
        errorMessage = "Cannot open the file!";
        return -1;
    }

    // 获取源文件大小
    stat(path.c_str(), &statbuf);
    filesize = statbuf.st_size;

    // 读取文件内容
    char *buf = new char[filesize];
    count_r = read(fp_r, buf, filesize);
    if (count_r == -1)
    {
        errorMessage = "Error reading the file!";
        delete[] buf;
        close(fp_r);
        return -1;
    }

    // 获取当前时间，创建时间戳
    time_s = getTime();  // 假设 getTime() 是你自定义的获取时间函数
    QString timeStamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // 创建文件包路径
    sprintf(filepackage, "/home/qs/our_packup/%s", name.toStdString().c_str());
    sprintf(filepath, "/home/qs/our_packup/%s/%s", name.toStdString().c_str(), time_s);

    // 检查目标目录是否存在，不存在则创建
    if (access(filepackage, X_OK) != 0)
    {
        if (mkdir(filepackage, 0755) == -1)
        {
            errorMessage = "Error creating directory!";
            delete[] buf;
            close(fp_r);
            return -1;
        }
    }

    // 创建目标文件
    fp_w = ::open(filepath, O_CREAT | O_RDWR, 0644);
    if (fp_w == -1)
    {
        errorMessage = "Error creating the target file!";
        delete[] buf;
        close(fp_r);
        return -1;
    }

    // 将文件内容写入目标文件
    count_w = write(fp_w, buf, filesize);
    if (count_w == -1)
    {
        errorMessage = "Error writing to the file!";
        delete[] buf;
        close(fp_r);
        close(fp_w);
        return -1;
    }

    // 清理资源
    delete[] time_s;
    delete[] buf;
    close(fp_r);
    close(fp_w);

    // 插入上传记录到数据库
    DatabaseManager& dbManager = DatabaseManager::instance();
    if (!dbManager.insertUploadRecord(QString::fromStdString(filepath), filesize, QString::fromStdString(path), name, timeStamp, errorMessage)) {
        return -1;  // 插入数据库记录失败
    }
    // 返回成功
    return 1;
}


int download(const std::string &path_user, const std::string &package_path)
{
    int fp_w;
    int fp_r;
    QString filename1;
    int filesize{};
    int count_r,count_w;
    //  char path[100];

    QPair<QString, int> result = DatabaseManager::instance().queryHistory(path_user);

    if (result.first.isEmpty()) {
        printf("No data found for the specified path.\n");
        return -1;  // 如果没有查询到数据，返回错误
    }

    filename1 = result.first;  // 文件名
    filesize = result.second;   // 文件大小

    char buf[filesize];
    char* filename = getFileName(filename1.toLatin1().data());
    std::string path = package_path + filename;
    printf("%s\n",path.c_str());
    fp_r = ::open(path_user.c_str(),O_RDONLY);
    if(fp_r == -1)
        printf("The file_r was not create!\n");
    if((count_r = read(fp_r,buf,filesize)<0))
        printf("read error\n");

    if(creat(path.c_str(),0777)<0)
    {
        printf("create error\n");
    }
    fp_w = ::open(path.c_str(),O_CREAT|O_RDWR,777);
    if(fp_w == -1)
        printf("The file_w was not create!\n");
    if((count_w = write(fp_w,buf,filesize)<0))
        printf("write error\n");

    close(fp_r);
    close(fp_w);

    return 1;
}

char *getFileName(char *path)
{
    char *name;
    char temp = '0';
    int i = 0, j = 0;

    name = (char *)malloc(sizeof(char) * 33);
    while (temp != '\0')
    {
        temp = path[i++];
        if (temp == '/')
        {
            j = 0;
        }
        else
        {
            name[j++] = temp;
        }
    }
    name[j] = '\0';
    return name;
}
