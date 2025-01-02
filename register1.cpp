#include "register1.h"
#include "ui_register1.h"
#include <QMessageBox>

register1::register1(DatabaseManager *dbManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::register1)
    , dbManager(dbManager)
{
    ui->setupUi(this);

    this->setWindowTitle("backup -- register");
}

register1::~register1()
{
    delete ui;
}

void register1::on_register_btn_clicked()
{
    QString username = ui->username->text();
    QString password = ui->psd->text();

    // 检查用户名和密码是否为空
    if (username.isEmpty()) {
        QMessageBox::warning(this, "Register", "Username cannot be empty.");
        return;
    }else if (password.isEmpty()){
        QMessageBox::warning(this, "Register", "Password cannot be empty.");
        return;
    }

    QString errorMessage;
    // 调用 DatabaseManager 的注册功能并获取错误信息
    if (dbManager->registerUser(username, password, errorMessage)) {
        QMessageBox::information(this, "Register", "Registration successful");
        this->accept();  // 注册成功，关闭窗口
    } else {
        QMessageBox::warning(this, "Register", errorMessage);  // 显示具体的错误信息
    }
}

void register1::on_backup_btn_clicked()
{
    this->reject();  // 点击取消按钮时，关闭窗口
}
