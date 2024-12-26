#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <qfiledialog.h>
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
