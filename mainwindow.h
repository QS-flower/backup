#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <iostream>
#include <QApplication>
#include <QProcess>
#include <QMessageBox>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "state.h"
#include "fileinformation.h"



// #include "filecompressor.h"

#define DATA_FILE_NAME "DATA.BACKUP"
#define UNIT_FILE_NAME "UNIT.BACKUP"

struct FIleUnitInfo //文件元信息结构体
{
    off_t TotalLength;
    ino_t Inode;
    nlink_t InodeCount;
    FileType Type;
    short int Auth;
    timespec Atime;
    uid_t UserID;
    gid_t GroupID;
    off_t Size;
    off_t StartOffset;
    std::string RelativePath;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_backup_btn_clicked();

    void on_restore_btn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H


