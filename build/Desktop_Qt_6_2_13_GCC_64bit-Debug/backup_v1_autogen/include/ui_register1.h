/********************************************************************************
** Form generated from reading UI file 'register1.ui'
**
** Created by: Qt User Interface Compiler version 6.2.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTER1_H
#define UI_REGISTER1_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_register1
{
public:
    QLabel *title;
    QLineEdit *username;
    QLineEdit *psd;
    QLineEdit *psd_again;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *register_btn;
    QSpacerItem *horizontalSpacer;
    QPushButton *backup_btn;
    QLabel *user_lab;
    QLabel *psd_lab;
    QLabel *psd_a_lab;
    QLabel *pic;

    void setupUi(QDialog *register1)
    {
        if (register1->objectName().isEmpty())
            register1->setObjectName(QString::fromUtf8("register1"));
        register1->resize(640, 380);
        title = new QLabel(register1);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(370, -20, 231, 71));
        QFont font;
        font.setFamilies({QString::fromUtf8("Arial")});
        font.setBold(true);
        title->setFont(font);
        title->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-family: 'Arial', sans-serif;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: #333;\n"
"    text-align: center;\n"
"    padding: 20px;\n"
"}"));
        title->setAlignment(Qt::AlignmentFlag::AlignCenter);
        username = new QLineEdit(register1);
        username->setObjectName(QString::fromUtf8("username"));
        username->setGeometry(QRect(380, 80, 211, 51));
        username->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: white;\n"
"    border: 1px solid #ccc;\n"
"    border-radius: 4px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #007BFF;\n"
"}"));
        username->setAlignment(Qt::AlignmentFlag::AlignCenter);
        psd = new QLineEdit(register1);
        psd->setObjectName(QString::fromUtf8("psd"));
        psd->setGeometry(QRect(380, 170, 211, 51));
        psd->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: white;\n"
"    border: 1px solid #ccc;\n"
"    border-radius: 4px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #007BFF;\n"
"}"));
        psd->setEchoMode(QLineEdit::EchoMode::Password);
        psd->setAlignment(Qt::AlignmentFlag::AlignCenter);
        psd_again = new QLineEdit(register1);
        psd_again->setObjectName(QString::fromUtf8("psd_again"));
        psd_again->setGeometry(QRect(380, 260, 211, 51));
        psd_again->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: white;\n"
"    border: 1px solid #ccc;\n"
"    border-radius: 4px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #007BFF;\n"
"}"));
        psd_again->setEchoMode(QLineEdit::EchoMode::Password);
        psd_again->setAlignment(Qt::AlignmentFlag::AlignCenter);
        horizontalLayoutWidget = new QWidget(register1);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(380, 320, 211, 51));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        register_btn = new QPushButton(horizontalLayoutWidget);
        register_btn->setObjectName(QString::fromUtf8("register_btn"));
        register_btn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #28a745;\n"
"    color: white;\n"
"    border-radius: 4px;\n"
"    padding: 12px 24px;\n"
"    font-size: 16px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #218838;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #1e7e34;\n"
"}"));

        horizontalLayout->addWidget(register_btn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        backup_btn = new QPushButton(horizontalLayoutWidget);
        backup_btn->setObjectName(QString::fromUtf8("backup_btn"));
        backup_btn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #dc3545;\n"
"    color: white;\n"
"    border-radius: 4px;\n"
"    padding: 12px 24px;\n"
"    font-size: 16px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #c82333;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #bd2130;\n"
"}"));

        horizontalLayout->addWidget(backup_btn);

        user_lab = new QLabel(register1);
        user_lab->setObjectName(QString::fromUtf8("user_lab"));
        user_lab->setGeometry(QRect(380, 50, 67, 19));
        QFont font1;
        font1.setPointSize(12);
        user_lab->setFont(font1);
        user_lab->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        user_lab->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        psd_lab = new QLabel(register1);
        psd_lab->setObjectName(QString::fromUtf8("psd_lab"));
        psd_lab->setGeometry(QRect(380, 140, 67, 19));
        psd_lab->setFont(font1);
        psd_lab->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        psd_lab->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        psd_a_lab = new QLabel(register1);
        psd_a_lab->setObjectName(QString::fromUtf8("psd_a_lab"));
        psd_a_lab->setGeometry(QRect(380, 230, 67, 19));
        psd_a_lab->setFont(font1);
        psd_a_lab->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        psd_a_lab->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        pic = new QLabel(register1);
        pic->setObjectName(QString::fromUtf8("pic"));
        pic->setGeometry(QRect(0, 0, 361, 381));
        pic->setStyleSheet(QString::fromUtf8("border-image: url(:/images/wel.png);"));

        retranslateUi(register1);

        QMetaObject::connectSlotsByName(register1);
    } // setupUi

    void retranslateUi(QDialog *register1)
    {
        register1->setWindowTitle(QCoreApplication::translate("register1", "Dialog", nullptr));
        title->setText(QCoreApplication::translate("register1", "Welcome", nullptr));
        username->setPlaceholderText(QCoreApplication::translate("register1", "\350\257\267\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215", nullptr));
        psd->setText(QString());
        psd->setPlaceholderText(QCoreApplication::translate("register1", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        psd_again->setPlaceholderText(QCoreApplication::translate("register1", "\350\257\267\345\206\215\346\254\241\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        register_btn->setText(QCoreApplication::translate("register1", "\346\263\250\345\206\214", nullptr));
        backup_btn->setText(QCoreApplication::translate("register1", "\350\277\224\345\233\236", nullptr));
        user_lab->setText(QCoreApplication::translate("register1", "\347\224\250\346\210\267\345\220\215", nullptr));
        psd_lab->setText(QCoreApplication::translate("register1", "\345\257\206\347\240\201", nullptr));
        psd_a_lab->setText(QCoreApplication::translate("register1", "\347\241\256\350\256\244\345\257\206\347\240\201", nullptr));
        pic->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class register1: public Ui_register1 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER1_H
