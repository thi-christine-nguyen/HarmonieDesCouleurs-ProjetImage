/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *Import;
    QLabel *ImgIn;
    QLabel *ImgOut;
    QPushButton *Monochromatique;
    QPushButton *Complementaire;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        Import = new QPushButton(centralwidget);
        Import->setObjectName("Import");
        Import->setGeometry(QRect(140, 440, 80, 24));
        ImgIn = new QLabel(centralwidget);
        ImgIn->setObjectName("ImgIn");
        ImgIn->setGeometry(QRect(30, 70, 301, 331));
        ImgIn->setFrameShape(QFrame::Box);
        ImgOut = new QLabel(centralwidget);
        ImgOut->setObjectName("ImgOut");
        ImgOut->setGeometry(QRect(460, 70, 301, 331));
        ImgOut->setFrameShape(QFrame::Box);
        Monochromatique = new QPushButton(centralwidget);
        Monochromatique->setObjectName("Monochromatique");
        Monochromatique->setGeometry(QRect(340, 100, 111, 31));
        Complementaire = new QPushButton(centralwidget);
        Complementaire->setObjectName("Complementaire");
        Complementaire->setGeometry(QRect(340, 150, 111, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        Import->setText(QCoreApplication::translate("MainWindow", "Import", nullptr));
        ImgIn->setText(QString());
        ImgOut->setText(QString());
        Monochromatique->setText(QCoreApplication::translate("MainWindow", "Monochromatique", nullptr));
        Complementaire->setText(QCoreApplication::translate("MainWindow", "Complementaire", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
