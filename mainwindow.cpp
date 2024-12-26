#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("backup pack");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_backup_btn_clicked()
{
    // 提示用户选择备份类型：文件夹或文件
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Backup Type");
    msgBox.setText("What do you want to backup?");
    msgBox.setIcon(QMessageBox::Question);

    // 添加按钮：文件夹和文件
    QPushButton *folderButton = msgBox.addButton("Folder", QMessageBox::ActionRole);
    QPushButton *fileButton = msgBox.addButton("File", QMessageBox::ActionRole);
    msgBox.addButton(QMessageBox::Cancel); // 添加取消按钮

    // 显示对话框并等待用户选择
    msgBox.exec();

    QString path;

    // 根据用户选择执行不同逻辑
    if (msgBox.clickedButton() == folderButton) {
        // 用户选择文件夹
        path = QFileDialog::getExistingDirectory(this, "Select Folder to Backup");
        if (!path.isEmpty()) {
            QMessageBox::information(this, "Folder Selected", "Selected folder: " + path);
        }
    } else if (msgBox.clickedButton() == fileButton) {
        // 用户选择文件
        path = QFileDialog::getOpenFileName(this, "Select File to Backup");
        if (!path.isEmpty()) {
            QMessageBox::information(this, "File Selected", "Selected file: " + path);
        }
    } else {
        // 用户选择取消或关闭窗口
        return;
    }

    // 如果用户未选择任何路径，什么都不做
    if (path.isEmpty()) {
        return;
    }
}


void MainWindow::on_restore_btn_clicked()
{

}

