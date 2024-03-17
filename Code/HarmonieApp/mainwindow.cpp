#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "image_ppm.h"
#include "convertisseur.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QImageReader>
#include <QPixmap>
#include <vector>
#include <iostream>

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

void MainWindow::on_GenerateHSV_clicked(){

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
        double R,G,B;
        R = (double)ImgIn[i]/255;
        G = (double)ImgIn[i+1]/255;
        B = (double)ImgIn[i+2]/255;
        double cmax = fmax(R,fmax(G,B));
        double cmin = fmin(R,fmin(G,B));
        double delta = cmax - cmin;
        double H = 0;

        if (delta == 0) {
            H = 0;
        } else if (cmax == R) {
            H = 60 * ((G - B) / delta);
        } else if (cmax == G) {
            H = 60 * (((B - R) / delta) + 2);
        } else if (cmax == B) {
            H = 60 * (((R - G) / delta) + 4);
        }
        if (H<0)
            H += 360;

        float S;
        if ( cmax ==0){
            S = 0;
        } else
            S = delta/cmax;

        ImgOut[i]= H;
        ImgOut[i+1]= S*100;
        ImgOut[i+2]= cmax*100;

    }

    free(ImgIn);


    char nomImgOut[] = "ImgOut";

    ecrire_image_ppm(nomImgOut, ImgOut,  nH, nW);

    // Set the pixmap to the label
    ui->ImgOut->setPixmap( QPixmap (QString::fromUtf8(nomImgOut)));
    ui->ImgOut->setScaledContents( true );
    ui->ImgOut->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

}


void MainWindow::on_FindBestHarmonie_clicked()
{

    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOutInter, *ImgOut, *ImgHsvRgb;

    char nom_image[ImgInPath.length() + 1];
    strcpy(nom_image, ImgInPath.toUtf8().constData());

    lire_nb_lignes_colonnes_image_ppm(nom_image, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(nom_image, ImgIn, nH * nW);
    allocation_tableau(ImgOutInter, OCTET, nTaille3);
    allocation_tableau(ImgHsvRgb, OCTET, nTaille3);

    for (int i=0; i < nTaille3; i+=3)
    {
        double R,G,B;
        R = (double)ImgIn[i]/255;
        G = (double)ImgIn[i+1]/255;
        B = (double)ImgIn[i+2]/255;
        double cmax = fmax(R,fmax(G,B));
        double cmin = fmin(R,fmin(G,B));
        double delta = cmax - cmin;
        double H = 0;

        if (delta == 0) {
            H = 0;
        } else if (cmax == R) {
            H = 60 * ((G - B) / delta);
        } else if (cmax == G) {
            H = 60 * (((B - R) / delta) + 2);
        } else if (cmax == B) {
            H = 60 * (((R - G) / delta) + 4);
        }
        if (H<0)
            H += 360;

        float S;
        if ( cmax ==0){
            S = 0;
        } else
            S = delta/cmax;

        ImgOutInter[i]= H;
        ImgOutInter[i+1]= S*100;
        ImgOutInter[i+2]= cmax*100;

    }

    std::vector<int> histogramme = getHistoHSV(ImgOutInter, nTaille3);

    ImgOut = findBestHarmonieCompl(histogramme, ImgIn, nTaille3);

    // for (int i = 0; i < nTaille3; i += 3) {
    //     int R, G, B;
    //     float H = ImgOut[i];
    //     float S = ImgOut[i+1] / 100.0;
    //     float V = ImgOut[i+2] / 100.0;
    //     HSVtoRGB(H, S, V, &R, &G, &B);
    //     ImgHsvRgb[i] = R;
    //     ImgHsvRgb[i+1] = G;
    //     ImgHsvRgb[i+2] = B;
    // }

    char nomImgOut[] = "ImgOut";

    ecrire_image_ppm(nomImgOut, ImgOut,  nH, nW);

    // Set the pixmap to the label
    ui->ImgOut->setPixmap( QPixmap (QString::fromUtf8(nomImgOut)));
    ui->ImgOut->setScaledContents( true );
    ui->ImgOut->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );


}

