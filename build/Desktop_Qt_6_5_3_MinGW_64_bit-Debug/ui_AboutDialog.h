/********************************************************************************
** Form generated from reading UI file 'AboutDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;
    QLabel *nameLabel;
    QLabel *teacherLabel;
    QLabel *versionLabel;
    QLabel *Author;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QString::fromUtf8("AboutDialog"));
        AboutDialog->resize(600, 500);
        horizontalLayout = new QHBoxLayout(AboutDialog);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        scrollArea = new QScrollArea(AboutDialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 586, 486));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        nameLabel = new QLabel(scrollAreaWidgetContents);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        QFont font;
        font.setFamily(QString::fromUtf8("Agency FB"));
        font.setBold(true);
        nameLabel->setFont(font);

        gridLayout->addWidget(nameLabel, 0, 0, 1, 1);

        teacherLabel = new QLabel(scrollAreaWidgetContents);
        teacherLabel->setObjectName(QString::fromUtf8("teacherLabel"));
        teacherLabel->setFont(font);

        gridLayout->addWidget(teacherLabel, 1, 0, 1, 1);

        versionLabel = new QLabel(scrollAreaWidgetContents);
        versionLabel->setObjectName(QString::fromUtf8("versionLabel"));
        versionLabel->setFont(font);

        gridLayout->addWidget(versionLabel, 0, 1, 1, 1);

        Author = new QLabel(scrollAreaWidgetContents);
        Author->setObjectName(QString::fromUtf8("Author"));
        Author->setFont(font);

        gridLayout->addWidget(Author, 2, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout->addWidget(scrollArea);


        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QCoreApplication::translate("AboutDialog", "Dialog", nullptr));
        nameLabel->setText(QCoreApplication::translate("AboutDialog", "\351\241\271\347\233\256\345\220\215\347\247\260\357\274\232", nullptr));
        teacherLabel->setText(QCoreApplication::translate("AboutDialog", "\346\214\207\345\257\274\350\200\201\345\270\210\357\274\232\345\255\231\345\256\207", nullptr));
        versionLabel->setText(QCoreApplication::translate("AboutDialog", "\345\275\223\345\211\215\347\211\210\346\234\254\357\274\232", nullptr));
        Author->setText(QCoreApplication::translate("AboutDialog", "\344\275\234\350\200\205\357\274\232\346\237\263\346\230\225\345\275\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
