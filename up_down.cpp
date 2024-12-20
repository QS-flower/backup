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

int upload(const std::string &path, const std::string &name)
{
    int fp_r;
    int fp_w;
    long filesize{0};
    char *time_s;
    int count_r,count_w;

    struct stat statbuf;

    char filepath[100];
    char filepackage[100];

    if((fp_r = ::open(path.c_str(),O_RDONLY))== -1)
    {
        printf("cannot open the file\n");
    }
    stat(path.c_str(),&statbuf);
    filesize = statbuf.st_size;

    char buf[filesize];

    count_r = read(fp_r,buf,filesize);
    if(count_r == -1)
        printf("read error!\n");

    time_s = getTime();
    printf("%s\n",time_s);

    sprintf(filepackage,"/home/qs/our_packup/%s",name.c_str());
    sprintf(filepath,"/home/qs/our_packup/%s/%s",name.c_str(),time_s);
    if(access(filepackage,X_OK) != 0)
    {
        if(mkdir(filepackage,0755) == -1)
        {
            printf("mkdir error\n");
            return -1;
        }
    }

    fp_w = ::open(filepath,O_CREAT|O_RDWR,777);
    if(fp_w == -1)
        printf("The file was not create!\n");

    count_w = write(fp_w,buf,filesize);

    printf("%s",buf);

    if(count_w == -1)
        printf("errono=%d\n",errno);

    delete[] time_s;
    time_s = nullptr;

    close(fp_r);
    close(fp_w);

    // 数据库记录
    // QString S = QString("insert into history(Time,Path,size,oldpath,user) values ('%1','%2','%3','%4','%5')" ).arg(time_s).arg(filepath).arg(filesize).arg(path.c_str()).arg(name.c_str());
    // QSqlQuery query;
    // if(query.exec(S) == true)
    //     printf("good\n");
    // else
    //     printf("fail\n");
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

    QString S = QString("select *from history where path = '%1'").arg(path_user.c_str());
    QSqlQuery query;
    if(query.exec(S) == true)
        printf("good\n");
    else
        printf("fail\n");
    if(query.next())
    {
        filename1 = query.value(4).toString();
        filesize = query.value(3).toInt();
    }
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
