/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.2.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QLabel *title;
    QLineEdit *username;
    QLineEdit *password;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *login_btn;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *register_btn;
    QLabel *user_lab;
    QLabel *user_lab_2;
    QLabel *pic;

    void setupUi(QDialog *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName(QString::fromUtf8("login"));
        login->resize(640, 360);
        title = new QLabel(login);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(360, 0, 231, 41));
        QFont font;
        font.setFamilies({QString::fromUtf8("Arial")});
        font.setBold(true);
        title->setFont(font);
        title->setStyleSheet(QString::fromUtf8("QLabel {\n"
"                                  font-family: 'Arial', sans-serif;\n"
"                                  font-size: 30px;\n"
"                                  font-weight: bold;\n"
"                                  color: #007BFF;\n"
"                                  text-align: center;\n"
"                                  text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"                                  }"));
        title->setTextFormat(Qt::TextFormat::AutoText);
        title->setAlignment(Qt::AlignmentFlag::AlignCenter);
        username = new QLineEdit(login);
        username->setObjectName(QString::fromUtf8("username"));
        username->setGeometry(QRect(350, 80, 251, 50));
        QFont font1;
        username->setFont(font1);
        username->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"                                         background-color: white;\n"
"                                         border: 1px solid #ccc;\n"
"                                         border-radius: 4px;\n"
"                                         padding: 10px;\n"
"                                         font-size: 16px;\n"
"                                         }"));
        username->setAlignment(Qt::AlignmentFlag::AlignCenter);
        password = new QLineEdit(login);
        password->setObjectName(QString::fromUtf8("password"));
        password->setGeometry(QRect(350, 170, 251, 50));
        password->setFont(font1);
        password->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"                                         background-color: white;\n"
"                                         border: 1px solid #ccc;\n"
"                                         border-radius: 4px;\n"
"                                         padding: 10px;\n"
"                                         font-size: 16px;\n"
"                                         }"));
        password->setEchoMode(QLineEdit::EchoMode::Password);
        password->setAlignment(Qt::AlignmentFlag::AlignCenter);
        horizontalLayoutWidget = new QWidget(login);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(350, 240, 251, 50));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        login_btn = new QPushButton(horizontalLayoutWidget);
        login_btn->setObjectName(QString::fromUtf8("login_btn"));
        login_btn->setEnabled(true);
        login_btn->setFont(font1);
        login_btn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"                                   background-color: #007BFF;\n"
"                                   color: white;\n"
"                                   border-radius: 4px;\n"
"                                   padding: 12px 24px;\n"
"                                   font-size: 16px;\n"
"                                   }\n"
"                                   QPushButton:hover {\n"
"                                   background-color: #0056b3;\n"
"                                   }\n"
"                                   QPushButton:pressed {\n"
"                                   background-color: #003f7f;\n"
"                                   }"));

        horizontalLayout_2->addWidget(login_btn);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));

        horizontalLayout_2->addLayout(horizontalLayout_3);

        register_btn = new QPushButton(horizontalLayoutWidget);
        register_btn->setObjectName(QString::fromUtf8("register_btn"));
        register_btn->setFont(font1);
        register_btn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"                                       background-color: #28a745;\n"
"                                       color: white;\n"
"                                       border-radius: 4px;\n"
"                                       padding: 12px 24px;\n"
"                                       font-size: 16px;\n"
"                                       }\n"
"                                       QPushButton:hover {\n"
"                                       background-color: #218838;\n"
"                                       }\n"
"                                       QPushButton:pressed {\n"
"                                       background-color: #1e7e34;\n"
"                                       }"));

        horizontalLayout_2->addWidget(register_btn);

        user_lab = new QLabel(login);
        user_lab->setObjectName(QString::fromUtf8("user_lab"));
        user_lab->setGeometry(QRect(350, 50, 67, 21));
        QFont font2;
        font2.setPointSize(12);
        user_lab->setFont(font2);
        user_lab->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        user_lab->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        user_lab_2 = new QLabel(login);
        user_lab_2->setObjectName(QString::fromUtf8("user_lab_2"));
        user_lab_2->setGeometry(QRect(350, 140, 67, 21));
        user_lab_2->setFont(font2);
        user_lab_2->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        user_lab_2->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        pic = new QLabel(login);
        pic->setObjectName(QString::fromUtf8("pic"));
        pic->setGeometry(QRect(0, 0, 331, 361));
        pic->setStyleSheet(QString::fromUtf8("border-image: url(:/images/wel.png);"));

        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QDialog *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "Dialog", nullptr));
        title->setText(QCoreApplication::translate("login", "LOGIN NOW", nullptr));
        username->setPlaceholderText(QCoreApplication::translate("login", "\350\257\267\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215", nullptr));
        password->setPlaceholderText(QCoreApplication::translate("login", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        login_btn->setText(QCoreApplication::translate("login", "\347\231\273\345\275\225", nullptr));
        register_btn->setText(QCoreApplication::translate("login", "\346\263\250\345\206\214", nullptr));
        user_lab->setText(QCoreApplication::translate("login", "\347\224\250\346\210\267\345\220\215", nullptr));
        user_lab_2->setText(QCoreApplication::translate("login", "\345\257\206\347\240\201", nullptr));
        pic->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
