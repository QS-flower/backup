#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

login::login(DatabaseManager *dbManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
    , dbManager(dbManager)  // 使用共享的 DatabaseManager 实例
{
    ui->setupUi(this);

    this->setWindowTitle("backup -- login");

    // 打开数据库
    if (!dbManager->openDatabase()) {
        QMessageBox::critical(this, "Error", "Failed to connect to database");
    }
}

login::~login()
{
    delete ui;
}

void login::on_login_btn_clicked()
{
    QString username = ui->username->text();
    QString password = ui->password->text();

    // 检查用户名和密码是否为空
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login", "Username and Password cannot be empty");
        return;
    }

    QString errorMessage;
    if (dbManager->authenticateUser(username, password, errorMessage)) {
        QMessageBox::information(this, "Login", "Login successful");
        // 这里可以进一步进入应用程序的主界面
        // 创建主窗口并显示
        MainWindow *mainWindow = new MainWindow(username);
        mainWindow->show();

        // 关闭登录窗口
        this->close();
    } else {
        // 登录失败，根据不同的错误信息返回不同的提示
        QMessageBox::warning(this, "Login", errorMessage);
    }
}

void login::on_register_btn_clicked()
{
    register1 register1(dbManager, this);  // 创建注册窗口对象
    if (register1.exec() == QDialog::Accepted) {
        // 如果注册窗口返回 Accepted，则说明注册成功
        QMessageBox::information(this, "Register", "Please login now");
    }
}
