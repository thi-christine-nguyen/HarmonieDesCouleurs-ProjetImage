#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "image_ppm.h"
#include "convertisseur.h"
#include "file_type.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QColor>
#include <QPalette>
#include <QMessageBox>
#include <QDir>
#include <QImageReader>
#include <QPixmap>
#include <vector>
#include <iostream>

QString ImgInPath;
QString ImgOutPath;
QColor colorValue;


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
    colorValue.setHsv(0, 0, 0);

    QFileInfo fileInfo(fileName);
    if(fileInfo.suffix() == "ppm"){
        ImgInPath = fileName;
    }else{
        QString ImgOutConvert = "ImgIn";
        ToPPM(fileName.toStdString(), ImgOutConvert.toStdString());

        QFileInfo fileInfoConvert(ImgOutConvert);
        ImgInPath = fileInfoConvert.absoluteFilePath();
        ImgInPath += ".ppm";
    }

    ui->ImgIn->setPixmap( QPixmap( fileName ) );
    ui->ImgIn->setScaledContents( true );
    ui->ImgIn->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

}

void MainWindow::on_Monochromatique_clicked()
{

    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgHsvRgb;

    char nom_image[ImgInPath.length() + 1];
    strcpy(nom_image, ImgInPath.toUtf8().constData());
    lire_nb_lignes_colonnes_image_ppm(nom_image, &nH, &nW);

    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(nom_image, ImgIn, nH * nW);
    allocation_tableau(ImgHsvRgb, OCTET, nTaille3);

    std::vector<float> ImgOutHSV;


    ImgOutHSV = RGBtoHSV(nTaille, ImgIn);

    std::vector<int> histogramme = getHistoHSV(ImgOutHSV, nTaille3);

    std::vector<float> harmonie;

    harmonie = findBestHarmonieMono(histogramme, ImgOutHSV, nTaille3, colorValue);

    for(int i = 0; i < harmonie.size(); i+=3){
        int r;
        int g;
        int b;
        HSVtoRGB(harmonie[i], harmonie[i+1], harmonie[i+2], &r, &g, &b);
        ImgHsvRgb[i] = r;
        ImgHsvRgb[i+1] = g;
        ImgHsvRgb[i+2] = b;


    }



    char nomImgOut[] = "ImgOut.ppm";

    QString ImgOutConvert = nomImgOut;
    QFileInfo fileInfoConvert(ImgOutConvert);
    ImgOutPath = fileInfoConvert.absoluteFilePath();




    ecrire_image_ppm(nomImgOut, ImgHsvRgb, nH, nW);

    free(ImgIn);
    free(ImgHsvRgb);

    // Set the pixmap to the label
    ui->ImgOut->setPixmap( QPixmap (QString::fromUtf8(nomImgOut)));
    ui->ImgOut->setScaledContents( true );
    ui->ImgOut->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );


}


void MainWindow::on_Complementaire_clicked()
{

    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgHsvRgb;

    char nom_image[ImgInPath.length() + 1];
    strcpy(nom_image, ImgInPath.toUtf8().constData());

    lire_nb_lignes_colonnes_image_ppm(nom_image, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(nom_image, ImgIn, nH * nW);
    allocation_tableau(ImgHsvRgb, OCTET, nTaille3);

    std::vector<float> ImgOutHSV;

    ImgOutHSV = RGBtoHSV(nTaille, ImgIn);

    std::vector<int> histogramme = getHistoHSV(ImgOutHSV, nTaille3);

    std::vector<float> harmonie;

    // harmonie = findBestHarmonieCompl(histogramme, ImgOutHSV, nTaille3);
    harmonie = choosedCompl(histogramme, ImgOutHSV, nTaille3, colorValue);


    for(int i = 0; i < harmonie.size(); i+=3){
        int r;
        int g;
        int b;
        HSVtoRGB(harmonie[i], harmonie[i+1], harmonie[i+2], &r, &g, &b);
        ImgHsvRgb[i] = r;
        ImgHsvRgb[i+1] = g;
        ImgHsvRgb[i+2] = b;


    }

    char nomImgOut[] = "ImgOut.ppm";

    QString ImgOutConvert = nomImgOut;
    QFileInfo fileInfoConvert(ImgOutConvert);
    ImgOutPath = fileInfoConvert.absoluteFilePath();

    ecrire_image_ppm(nomImgOut, ImgHsvRgb, nH, nW);

    free(ImgIn);
    free(ImgHsvRgb);

    // Set the pixmap to the label
    ui->ImgOut->setPixmap( QPixmap (QString::fromUtf8(nomImgOut)));
    ui->ImgOut->setScaledContents( true );
    ui->ImgOut->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
}


void MainWindow::on_Triadique_clicked()
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgHsvRgb;

    char nom_image[ImgInPath.length() + 1];
    strcpy(nom_image, ImgInPath.toUtf8().constData());

    lire_nb_lignes_colonnes_image_ppm(nom_image, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(nom_image, ImgIn, nH * nW);
    allocation_tableau(ImgHsvRgb, OCTET, nTaille3);

    std::vector<float> ImgOutHSV;

    ImgOutHSV = RGBtoHSV(nTaille, ImgIn);

    std::vector<int> histogramme = getHistoHSV(ImgOutHSV, nTaille3);

    std::vector<float> harmonie;

    // harmonie = findBestHarmonieTri(histogramme, ImgOutHSV, nTaille3);
    harmonie = choosedHarmonieTri(histogramme, ImgOutHSV, nTaille3, colorValue);

    for(int i = 0; i < harmonie.size(); i+=3){
        int r;
        int g;
        int b;
        HSVtoRGB(harmonie[i], harmonie[i+1], harmonie[i+2], &r, &g, &b);
        ImgHsvRgb[i] = r;
        ImgHsvRgb[i+1] = g;
        ImgHsvRgb[i+2] = b;


    }

    char nomImgOut[] = "ImgOut.ppm";

    QString ImgOutConvert = nomImgOut;
    QFileInfo fileInfoConvert(ImgOutConvert);
    ImgOutPath = fileInfoConvert.absoluteFilePath();

    ecrire_image_ppm(nomImgOut, ImgHsvRgb, nH, nW);

    free(ImgIn);
    free(ImgHsvRgb);

    // Set the pixmap to the label
    ui->ImgOut->setPixmap( QPixmap (QString::fromUtf8(nomImgOut)));
    ui->ImgOut->setScaledContents( true );
    ui->ImgOut->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
}


void MainWindow::on_Analogue_clicked()
{

    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgHsvRgb;

    char nom_image[ImgInPath.length() + 1];
    strcpy(nom_image, ImgInPath.toUtf8().constData());

    lire_nb_lignes_colonnes_image_ppm(nom_image, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(nom_image, ImgIn, nH * nW);
    allocation_tableau(ImgHsvRgb, OCTET, nTaille3);

    std::vector<float> ImgOutHSV;

    ImgOutHSV = RGBtoHSV(nTaille, ImgIn);

    std::vector<int> histogramme = getHistoHSV(ImgOutHSV, nTaille3);

    std::vector<float> harmonie;

    harmonie = choosedHarmonieAnalogue(histogramme, ImgOutHSV, nTaille3, colorValue);

    for(int i = 0; i < harmonie.size(); i+=3){
        int r;
        int g;
        int b;
        HSVtoRGB(harmonie[i], harmonie[i+1], harmonie[i+2], &r, &g, &b);
        ImgHsvRgb[i] = r;
        ImgHsvRgb[i+1] = g;
        ImgHsvRgb[i+2] = b;


    }

    char nomImgOut[] = "ImgOut.ppm";

    QString ImgOutConvert = nomImgOut;
    QFileInfo fileInfoConvert(ImgOutConvert);
    ImgOutPath = fileInfoConvert.absoluteFilePath();

    ecrire_image_ppm(nomImgOut, ImgHsvRgb, nH, nW);

    free(ImgIn);
    free(ImgHsvRgb);

    // Set the pixmap to the label
    ui->ImgOut->setPixmap( QPixmap (QString::fromUtf8(nomImgOut)));
    ui->ImgOut->setScaledContents( true );
    ui->ImgOut->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
}



void MainWindow::on_Selectioncourleur_clicked()
{
    colorValue = QColorDialog::getColor(Qt::white, this, tr("Select Color"));
}


void MainWindow::on_Export_clicked()
{
    std::cout << ImgOutPath.toStdString() << std::endl;

    PPMtoJPEG(ImgOutPath.toStdString(), "output");

}

