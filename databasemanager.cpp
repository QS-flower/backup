#include "databasemanager.h"
#include <QDebug>
#include <QRegularExpression>
#include <QSqlQuery>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager()
{
    db = QSqlDatabase::addDatabase("QMYSQL", "my_connection");
}

DatabaseManager::~DatabaseManager()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::openDatabase()
{
    db.setHostName("localhost");
    db.setDatabaseName("backup");
    db.setUserName("root");
    db.setPassword("QJJ123456");

    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::isUsernameExist(const QString &username)
{
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;  // 如果用户名已经存在，返回 true
    }
    return false;
}

bool DatabaseManager::isPasswordStrong(const QString &password)
{
    // 密码强度检查规则：至少8个字符，包含大写字母，小写字母和数字
    QRegularExpression passwordRegExp("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[A-Za-z\\d]{8,}$");
    return passwordRegExp.match(password).hasMatch();  // 使用 QRegularExpression 进行匹配
}

bool DatabaseManager::authenticateUser(const QString &username, const QString &password, QString &errorMessage)
{
    QSqlQuery query(db);
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec()) {
        if (query.next()) {
            QString dbPassword = query.value(0).toString();
            if (dbPassword == password) {
                return true;  // 登录成功
            } else {
                errorMessage = "Incorrect password!";
                return false;  // 密码错误
            }
        } else {
            errorMessage = "Username not found!";
            return false;  // 用户名不存在
        }
    } else {
        errorMessage = "Database query failed!";
        return false;  // 查询失败
    }
}

bool DatabaseManager::registerUser(const QString &username, const QString &password, QString &errorMessage)
{
    // 检查用户名是否已存在
    if (isUsernameExist(username)) {
        errorMessage = "Username already exists!";
        return false;
    }

    // 检查密码是否符合强度要求
    if (!isPasswordStrong(password)) {
        errorMessage = "Password is too weak! It should contain at least 8 characters, including uppercase letters, lowercase letters, and digits.";
        return false;
    }

    // 如果用户名不存在且密码强度符合要求，则执行注册
    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec()) {
        return true;
    } else {
        errorMessage = "Registration failed: " + query.lastError().text();
        return false;
    }
}
//上传备份记录
bool DatabaseManager::insertUploadRecord(const QString &filepath, long filesize, const QString &oldpath, const QString &username, const QString &time_s, QString &errorMessage)
{
    QSqlQuery query(db);

    // 创建 SQL 插入语句
    query.prepare("INSERT INTO history (time, path, size, oldpath, user) "
                  "VALUES (:time, :path, :size, :oldpath, :user)");
    query.bindValue(":time", time_s);
    query.bindValue(":path", filepath);
    query.bindValue(":size", QVariant::fromValue(filesize));
    query.bindValue(":oldpath", oldpath);
    query.bindValue(":user", username);

    // 执行插入查询
    if (query.exec()) {
        return true;
    } else {
        errorMessage = "Failed to insert record into database: " + query.lastError().text();
        return false;
    }
}
QPair<QString, int> DatabaseManager::queryHistory(const std::string &path_user)
{
    QPair<QString, int> result;  // 用来存储查询结果（文件名和文件大小）

    QString queryStr = QString("SELECT * FROM history WHERE path = '%1'").arg(QString::fromStdString(path_user));
    QSqlQuery query(db);

    if (!query.exec(queryStr)) {
        qDebug() << "Query execution failed:" << query.lastError().text();
        return result;  // 返回默认构造的 QPair
    }

    if (query.next()) {
        // 获取查询结果，假设文件名存储在第5列（索引4），文件大小存储在第4列（索引3）
        result.first = query.value(4).toString();  // 文件名
        result.second = query.value(3).toInt();    // 文件大小
    }
    qDebug() << result;
    return result;
}
