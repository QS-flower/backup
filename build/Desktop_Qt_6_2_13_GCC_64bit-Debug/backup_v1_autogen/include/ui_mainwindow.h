/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QLabel *title;
    QLabel *wel;
    QLabel *wel1;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *backup_btn;
    QPushButton *restore_btn;
    QLabel *pic;

    void setupUi(QDialog *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(640, 360);
        title = new QLabel(MainWindow);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(360, 30, 221, 51));
        QFont font;
        font.setBold(true);
        title->setFont(font);
        title->setStyleSheet(QString::fromUtf8("QLabel {\n"
"            font-size: 24px;\n"
"            font-weight: bold;\n"
"            color: #2c3e50;\n"
"            background-color: #ecf0f1;\n"
"            padding: 10px;\n"
"            border: 2px solid #2980b9;\n"
"            border-radius: 8px;\n"
"            text-align: center;\n"
"        }"));
        title->setAlignment(Qt::AlignmentFlag::AlignCenter);
        wel = new QLabel(MainWindow);
        wel->setObjectName(QString::fromUtf8("wel"));
        wel->setGeometry(QRect(320, 80, 291, 51));
        QFont font1;
        wel->setFont(font1);
        wel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"            font-size: 18px;\n"
"            color: #16a085;\n"
"            margin: 10px 0;\n"
"            text-align: center;\n"
"        }"));
        wel1 = new QLabel(MainWindow);
        wel1->setObjectName(QString::fromUtf8("wel1"));
        wel1->setGeometry(QRect(310, 130, 161, 31));
        QFont font2;
        font2.setItalic(true);
        wel1->setFont(font2);
        wel1->setStyleSheet(QString::fromUtf8("QLabel {\n"
"            font-size: 14px;\n"
"            color: #e74c3c;\n"
"            font-style: italic;\n"
"            margin: 5px 0;\n"
"        }"));
        verticalLayoutWidget = new QWidget(MainWindow);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(340, 170, 251, 104));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        backup_btn = new QPushButton(verticalLayoutWidget);
        backup_btn->setObjectName(QString::fromUtf8("backup_btn"));
        backup_btn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-size: 16px;             /* \345\255\227\344\275\223\345\244\247\345\260\217 */\n"
"    font-weight: bold;           /* \345\212\240\347\262\227\345\255\227\344\275\223 */\n"
"    color: #ffffff;              /* \345\255\227\344\275\223\351\242\234\350\211\262 */\n"
"    background-color: #3498db;   /* \346\214\211\351\222\256\350\203\214\346\231\257\351\242\234\350\211\262 (\350\223\235\350\211\262) */\n"
"    border: 2px solid #2980b9;   /* \350\276\271\346\241\206\351\242\234\350\211\262 */\n"
"    border-radius: 10px;         /* \345\234\206\350\247\222\350\276\271\346\241\206 */\n"
"    padding: 10px 20px;          /* \345\206\205\350\276\271\350\267\235 */\n"
"}\n"
"\n"
"QPushButton#loginButton:hover {\n"
"    background-color: #2980b9;   /* \351\274\240\346\240\207\346\202\254\345\201\234\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QPushButton#loginButton:pressed {\n"
"    background-color: #1c6ea4;   /* \346\214\211\344\270\213\350\203\214\346\231\257\351"
                        "\242\234\350\211\262 */\n"
"}"));

        verticalLayout->addWidget(backup_btn);

        restore_btn = new QPushButton(verticalLayoutWidget);
        restore_btn->setObjectName(QString::fromUtf8("restore_btn"));
        restore_btn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-size: 16px;             /* \345\255\227\344\275\223\345\244\247\345\260\217 */\n"
"    font-weight: bold;           /* \345\212\240\347\262\227\345\255\227\344\275\223 */\n"
"    color: #ffffff;              /* \345\255\227\344\275\223\351\242\234\350\211\262 */\n"
"    background-color: #2ecc71;   /* \346\214\211\351\222\256\350\203\214\346\231\257\351\242\234\350\211\262 (\347\273\277\350\211\262) */\n"
"    border: 2px solid #27ae60;   /* \350\276\271\346\241\206\351\242\234\350\211\262 */\n"
"    border-radius: 10px;         /* \345\234\206\350\247\222\350\276\271\346\241\206 */\n"
"    padding: 10px 20px;          /* \345\206\205\350\276\271\350\267\235 */\n"
"}\n"
"\n"
"QPushButton#registerButton:hover {\n"
"    background-color: #27ae60;   /* \351\274\240\346\240\207\346\202\254\345\201\234\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QPushButton#registerButton:pressed {\n"
"    background-color: #1e8449;   /* \346\214\211\344\270\213\350\203\214\346\231\257"
                        "\351\242\234\350\211\262 */\n"
"}"));

        verticalLayout->addWidget(restore_btn);

        pic = new QLabel(MainWindow);
        pic->setObjectName(QString::fromUtf8("pic"));
        pic->setGeometry(QRect(0, 0, 301, 361));
        pic->setStyleSheet(QString::fromUtf8("border-image: url(:/images/main.png);"));

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QDialog *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Dialog", nullptr));
        title->setText(QCoreApplication::translate("MainWindow", "backup pack", nullptr));
        wel->setText(QCoreApplication::translate("MainWindow", "\346\254\242\350\277\216\344\275\277\347\224\250backup pack\346\226\207\344\273\266\345\244\207\344\273\275\345\267\245\345\205\267", nullptr));
        wel1->setText(QCoreApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251\346\202\250\347\232\204\346\223\215\344\275\234\357\274\232", nullptr));
        backup_btn->setText(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\345\244\207\344\273\275", nullptr));
        restore_btn->setText(QCoreApplication::translate("MainWindow", "\345\244\207\344\273\275\346\226\207\344\273\266\350\277\230\345\216\237", nullptr));
        pic->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
