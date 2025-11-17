/********************************************************************************
** Form generated from reading UI file 'Lab4.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAB4_H
#define UI_LAB4_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Lab4Class
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Lab4Class)
    {
        if (Lab4Class->objectName().isEmpty())
            Lab4Class->setObjectName("Lab4Class");
        Lab4Class->resize(600, 400);
        menuBar = new QMenuBar(Lab4Class);
        menuBar->setObjectName("menuBar");
        Lab4Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Lab4Class);
        mainToolBar->setObjectName("mainToolBar");
        Lab4Class->addToolBar(mainToolBar);
        centralWidget = new QWidget(Lab4Class);
        centralWidget->setObjectName("centralWidget");
        Lab4Class->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Lab4Class);
        statusBar->setObjectName("statusBar");
        Lab4Class->setStatusBar(statusBar);

        retranslateUi(Lab4Class);

        QMetaObject::connectSlotsByName(Lab4Class);
    } // setupUi

    void retranslateUi(QMainWindow *Lab4Class)
    {
        Lab4Class->setWindowTitle(QCoreApplication::translate("Lab4Class", "Lab4", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Lab4Class: public Ui_Lab4Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAB4_H
