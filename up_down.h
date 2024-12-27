#ifndef UP_DOWN_H
#define UP_DOWN_H

#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "login.h"
#include "databasemanager.h"
using namespace std;

struct historysend
{
    char id[17];
    char time[20];
    char path[129];
    char size[17];
};

int upload(const std::string &path, const QString &name, QString &errorMessage);
int download(const std::string &path_user,const std::string &package_path);
char *getFileName(char *path);
char *getTime();

#endif // UP_DOWN_H
