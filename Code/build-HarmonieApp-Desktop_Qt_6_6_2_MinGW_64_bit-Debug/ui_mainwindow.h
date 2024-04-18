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
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *Monochromatique;
    QPushButton *Complementaire;
    QPushButton *Triadique;
    QPushButton *Analogue;
    QPushButton *Selectioncourleur;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_3;
    QLabel *ImgOut;
    QPushButton *Export;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_2;
    QLabel *ImgIn;
    QPushButton *Import;
    QToolButton *colorButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(340, 110, 121, 271));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        Monochromatique = new QPushButton(layoutWidget);
        Monochromatique->setObjectName("Monochromatique");
        Monochromatique->setMinimumSize(QSize(111, 31));
        Monochromatique->setMaximumSize(QSize(111, 31));

        verticalLayout->addWidget(Monochromatique);

        Complementaire = new QPushButton(layoutWidget);
        Complementaire->setObjectName("Complementaire");
        Complementaire->setMinimumSize(QSize(111, 31));
        Complementaire->setMaximumSize(QSize(111, 31));

        verticalLayout->addWidget(Complementaire);

        Triadique = new QPushButton(layoutWidget);
        Triadique->setObjectName("Triadique");
        Triadique->setMinimumSize(QSize(111, 31));
        Triadique->setMaximumSize(QSize(111, 31));

        verticalLayout->addWidget(Triadique);

        Analogue = new QPushButton(layoutWidget);
        Analogue->setObjectName("Analogue");
        Analogue->setMinimumSize(QSize(111, 31));
        Analogue->setMaximumSize(QSize(111, 31));

        verticalLayout->addWidget(Analogue);

        Selectioncourleur = new QPushButton(layoutWidget);
        Selectioncourleur->setObjectName("Selectioncourleur");
        Selectioncourleur->setMinimumSize(QSize(111, 31));
        Selectioncourleur->setMaximumSize(QSize(111, 31));

        verticalLayout->addWidget(Selectioncourleur);

        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(480, 70, 301, 371));
        verticalLayout_3 = new QVBoxLayout(layoutWidget1);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        ImgOut = new QLabel(layoutWidget1);
        ImgOut->setObjectName("ImgOut");
        ImgOut->setFrameShape(QFrame::Box);

        verticalLayout_3->addWidget(ImgOut);

        Export = new QPushButton(layoutWidget1);
        Export->setObjectName("Export");

        verticalLayout_3->addWidget(Export);

        layoutWidget2 = new QWidget(centralwidget);
        layoutWidget2->setObjectName("layoutWidget2");
        layoutWidget2->setGeometry(QRect(20, 70, 301, 371));
        verticalLayout_2 = new QVBoxLayout(layoutWidget2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        ImgIn = new QLabel(layoutWidget2);
        ImgIn->setObjectName("ImgIn");
        ImgIn->setFrameShape(QFrame::Box);

        verticalLayout_2->addWidget(ImgIn);

        Import = new QPushButton(layoutWidget2);
        Import->setObjectName("Import");

        verticalLayout_2->addWidget(Import);

        colorButton = new QToolButton(centralwidget);
        colorButton->setObjectName("colorButton");
        colorButton->setGeometry(QRect(370, 420, 61, 61));
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
        Monochromatique->setText(QCoreApplication::translate("MainWindow", "Monochromatique", nullptr));
        Complementaire->setText(QCoreApplication::translate("MainWindow", "Complementaire", nullptr));
        Triadique->setText(QCoreApplication::translate("MainWindow", "Triadique", nullptr));
        Analogue->setText(QCoreApplication::translate("MainWindow", "Analogue", nullptr));
        Selectioncourleur->setText(QCoreApplication::translate("MainWindow", "Selection couleur", nullptr));
        ImgOut->setText(QString());
        Export->setText(QCoreApplication::translate("MainWindow", "Export", nullptr));
        ImgIn->setText(QString());
        Import->setText(QCoreApplication::translate("MainWindow", "Import", nullptr));
        colorButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
