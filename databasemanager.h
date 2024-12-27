#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    bool openDatabase();
    bool authenticateUser(const QString &username, const QString &password, QString &errorMessage);//登录并返回错误信息
    bool registerUser(const QString &username, const QString &password, QString &errorMessage);  // 注册并返回错误信息
    bool isUsernameExist(const QString &username);  // 检查用户名是否已存在
    bool isPasswordStrong(const QString &password);  // 检查密码强度
    //上传备份记录
    bool insertUploadRecord(const QString &filepath, long filesize, const QString &oldpath, const QString &username, const QString &time_s, QString &errorMessage);
    //下载时进行查询
    QPair<QString, int> queryHistory(const std::string &path_user);
    ~DatabaseManager();

private:
    DatabaseManager();
    QSqlDatabase db;
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
};

#endif // DATABASEMANAGER_H
