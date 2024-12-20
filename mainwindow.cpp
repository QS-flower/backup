#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsDropShadowEffect>
#include "filepacker.h"
#include "filecompressor.h"
#include "filereader.h"
#include "up_down.h"

#include <QGraphicsDropShadowEffect>

using namespace std;

//主页面，选择备份或者恢复文件
MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("backup pack");

    //设置图片
    /*
    QPixmap *pix = new QPixmap(":/******.png");
    QSize sz = ui->label_image->size();
    ui->label_image->setPixmap(pix->scaled(sz));
    */

    //设置图片阴影效果
    /*
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(3, 0);
    shadow->setColor(QColor("#888888"));
    shadow->setBlurRadius(30);
    ui->label_image->setGraphicsEffect(shadow);
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_backup_btn_clicked()//备份
{
    QString FileName;
    QMessageBox *messagebox  = new QMessageBox(this);
    messagebox->setBaseSize(600,300);
    messagebox->setWindowTitle("Select the data you want to backup：");
    messagebox->setText("Select file or directiory：");
    QPushButton *file =messagebox->addButton("FILE",QMessageBox::RejectRole);
    QPushButton *dir = messagebox->addButton("DIRECTORY",QMessageBox::AcceptRole);
    messagebox->exec();
    if(messagebox->clickedButton() == file)
        FileName = QFileDialog::getOpenFileName(this,"Choose file:","/home/czy");
    else if(messagebox->clickedButton() == dir)
        FileName = QFileDialog::getExistingDirectory(this,"Choose directory:","/home/czy");

    char tmpf[] = "/tmp/BackupFile.XXXXXX";
    QString tempDirectory = mkdtemp(tmpf);//创建存放数据的临时文件
    tempDirectory += "/";
    qDebug() << "tempDirectory: " << tempDirectory;//查看生成的临时文件
    if (!FileName.isEmpty()) {
        qDebug() << "FileName: " << FileName;//验证文件打开成功
    }
    int pos = FileName.lastIndexOf('/',FileName.size() - 2);

    QString RelativePath = FileName.right(FileName.size() - pos - 1);
    QString RootDirectory = FileName.left(pos+1);

    qDebug() << " RelativePath: " << RelativePath;//验证文件的文件名
    qDebug() << " RootDirectory: " << RootDirectory;//验证文件的上层根路径
    QMessageBox::information(NULL, "", "Compacting...");//文件打包
    FilePacker Packer = FilePacker(tempDirectory.toStdString(),true);
    delete InodeRecorder::inodeRecorderBackup;
    InodeRecorder::inodeRecorderBackup = new InodeRecorderBackup();//创建元数据节点
    FileInformation *fileInfo = new FileInformation(RelativePath.toStdString(),&Packer,RootDirectory.toStdString());
    if (fileInfo->Backup() != NO_ERROR)//验证备份成功
    {
        delete fileInfo;
        fileInfo = nullptr;
        return;
    }
    delete fileInfo;
    fileInfo = nullptr;
    Packer.CompactFile();
    QMessageBox::information(NULL, "", "File compacts over!");

    //文件压缩
    QMessageBox::information(NULL, "", "Compressing...");
    FileCompressor *fileCompressor = new FileCompressor(tempDirectory.toStdString(),Packer.BackupFile);
    if(fileCompressor->Compress() != NO_ERROR)
    {
        delete fileCompressor;
        printf("Compress fail！\n");
        return;
    }
    QMessageBox::information(NULL, "", "Compress finish!");

    //文件上传（为了测试，我们将name替换成具体的字符，实际上，name应该是从数据库中导出的，修改第103行name参数）
    std::string name = "Lz_test"; // 测试
    QMessageBox::information(NULL, "", "File Uploading...");
    if(upload((tempDirectory + COMPRESSOR_FILE_NAME).toStdString().c_str(),name.c_str()) == 1)
    {
        fileCompressor->DeleteFile();
        delete fileCompressor;
        Packer.DeleteFile();
        printf("ALL PACKUP PROCESS FINISHED SUCCESSFULLY!\n");
        system((string("rm -R ") + tempDirectory.toStdString()).c_str());
    }
    else
    {
        delete fileCompressor;
        printf("Backup failed! Because unknown error occurred on uploading.\n");
        printf("Backup File store in %s",tempDirectory);
    }
}


void MainWindow::on_restore_btn_clicked()
{

    char tr[] = "/tmp/BackupToolTmpFile.XXXXXX";
    QString tempDirectory = mkdtemp(tr);
    tempDirectory += "/";
    qDebug() << "tempDirectory: " << tempDirectory;
    //选择目录界面，并将所得目录传递给变量rootDirectory
    QString rootDirectory = QFileDialog::getExistingDirectory(this,"请选择目标文件夹","/home/qs");
    if (!rootDirectory.isEmpty()) {
        qDebug() << "rootDircetory: " << rootDirectory;
    }
    if (rootDirectory.back() != '/')
        rootDirectory += '/';

    //文件下载（为了测试，我们将name替换成具体的字符，实际上，name应该是从数据库中导出的，修改第139行name参数）
    QString name_test = "Lz_test"; // 测试


    QString packDirecroty_user = "/home/qs/our_packup/" + name_test;
    QString packDirectory = QFileDialog::getOpenFileName(this,"请选择文件",packDirecroty_user);
    if (!packDirectory.isEmpty()) {
        qDebug() << "packDirectory: " << packDirectory;
    }

    QMessageBox::information(NULL, "", "正在下载中...");
    if(download(packDirectory.toStdString().c_str(),tempDirectory.toStdString().c_str())==1){ //下载备份的文件
        QMessageBox::information(NULL, "", "文件下载成功！");
        //解压缩
        QMessageBox::information(NULL, "", "正在解压中...");
        FileCompressor *fileCompressor = new FileCompressor(tempDirectory.toStdString());
        if(fileCompressor->Decompress() != NO_ERROR)
        {
            delete fileCompressor;
            printf("解压失败！\n");
            return;
            //error
        }
        fileCompressor->DeleteFile();
        delete fileCompressor;
        QMessageBox::information(NULL, "", "文件解压成功！");
//             //解包
//             QMessageBox::information(NULL, "", "正在解包中...");
//             FilePacker Packer = FilePacker(tempDirectory.toStdString(),false);
//             Packer.Disassemble();
//             delete InodeRecorder::inodeRecorderRestore;
//             InodeRecorder::inodeRecorderRestore = new InodeRecorderRestore();
//             off_t ProcessBarTotal = Packer.UnitFile->Length();
//             off_t ProcessBarCurrent{0};
//             while(Packer.UnitFile->peek() != EOF)
//             {
//                 FileInfo *fileInfo = new FileInfo(&Packer,rootDirectory.toStdString());
//                 if(fileInfo->Restore() != NO_ERROR)
//                 {
//                     delete fileInfo;
//                     fileInfo = nullptr;
//                     QMessageBox::information(NULL, "", "文件恢复失败！");
//                     printf("恢复失败！\n");
//                     return;
//                     //error
//                 }
//                 char adjust = (ProcessBarCurrent>>20) & 0x1;
//                 ProcessBarCurrent = Packer.UnitFile->tellg();
//                 if(((ProcessBarCurrent>>20) & 0x1) == adjust + 1)
//                     cout << "\rRestoring: (" << ProcessBarCurrent << " / " << ProcessBarTotal << ")" << endl;
//                 delete fileInfo;
//                 fileInfo = nullptr;
//             }
//             QMessageBox::information(NULL, "", "文件解包完成！");
//             Packer.DeleteFile();
//             cout << "Restoring: (" << ProcessBarTotal << " / " << ProcessBarTotal << ")" << endl;
//             QMessageBox::information(NULL, "", "文件恢复成功！");
//             cout << "All restore process finished successfully!" << endl;
//             system((string("rm -R ") + tempDirectory.toStdString()).c_str());
//         }
//         else
//         {
//             printf("download fail!\n");
//             QMessageBox::warning(NULL, "", "文件下载失败！请重试。");
//             system((string("rm -R ") + tempDirectory.toStdString()).c_str());
//         }

    }

//     void choose::getData(QString str1)
//     {
//         //获取从mainwindow界面登录成功的用户名
//         name = str1;
//     }
//     // QMessageBox::information(this, "backup", "Please backup now");
}

// void MainWindow::on_restore_btn_clicked()
// {
//     QMessageBox::information(this, "restore", "Please restore now");
// }









