#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "image_ppm.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QImageReader>
#include <QPixmap>

QString ImgInPath;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Import_clicked()
{
    // Ouverture sur le dossier home
    QString fileName = QFileDialog::getOpenFileName(this, "Import", QDir::homePath());



    ImgInPath = fileName;

    ui->ImgIn->setPixmap( QPixmap( fileName ) );
    ui->ImgIn->setScaledContents( true );
    ui->ImgIn->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

}


void MainWindow::on_GenerateR_clicked()
{

    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    char nom_image[ImgInPath.length() + 1];
    strcpy(nom_image, ImgInPath.toUtf8().constData());

    lire_nb_lignes_colonnes_image_ppm(nom_image, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(nom_image, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille3);

    for (int i=0; i < nTaille3; i+=3)
    {
        if(ImgIn[i]<220)
            ImgOut[i]=ImgIn[i]+10;
        else
            ImgOut[i]=ImgIn[i];
    }
    free(ImgIn);


    char nomImgOut[] = "ImgOut";

    ecrire_image_ppm(nomImgOut, ImgOut,  nH, nW);

    // Set the pixmap to the label
    ui->ImgOut->setPixmap( QPixmap (QString::fromUtf8(nomImgOut)));
    ui->ImgOut->setScaledContents( true );
    ui->ImgOut->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );


}

