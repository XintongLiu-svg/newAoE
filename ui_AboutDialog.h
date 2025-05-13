/********************************************************************************
** Form generated from reading UI file 'AboutDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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
    QLabel *versionLabel;
    QLabel *teacherLabel;
    QLabel *Author;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QStringLiteral("AboutDialog"));
        AboutDialog->resize(600, 500);
        horizontalLayout = new QHBoxLayout(AboutDialog);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        scrollArea = new QScrollArea(AboutDialog);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 580, 480));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        nameLabel = new QLabel(scrollAreaWidgetContents);
        nameLabel->setObjectName(QStringLiteral("nameLabel"));
        QFont font;
        font.setFamily(QStringLiteral("Agency FB"));
        font.setBold(true);
        font.setWeight(75);
        nameLabel->setFont(font);

        gridLayout->addWidget(nameLabel, 0, 0, 1, 1);

        versionLabel = new QLabel(scrollAreaWidgetContents);
        versionLabel->setObjectName(QStringLiteral("versionLabel"));
        versionLabel->setFont(font);

        gridLayout->addWidget(versionLabel, 0, 1, 1, 1);

        teacherLabel = new QLabel(scrollAreaWidgetContents);
        teacherLabel->setObjectName(QStringLiteral("teacherLabel"));
        teacherLabel->setFont(font);

        gridLayout->addWidget(teacherLabel, 1, 0, 1, 1);

        Author = new QLabel(scrollAreaWidgetContents);
        Author->setObjectName(QStringLiteral("Author"));
        Author->setFont(font);
        gridLayout->addWidget(Author, 2, 0, 1, 2);

        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout->addWidget(scrollArea);


        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "Dialog", Q_NULLPTR));
        nameLabel->setText(QApplication::translate("AboutDialog", "项目名称：", Q_NULLPTR));
        versionLabel->setText(QApplication::translate("AboutDialog", "当前版本：", Q_NULLPTR));
        teacherLabel->setText(QApplication::translate("AboutDialog", "指导老师：孙宇", Q_NULLPTR));
        Author->setText(QApplication::translate("AboutDialog", "作者：柳昕彤", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
