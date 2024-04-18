#ifndef CONVERTISSEUR_H
#define CONVERTISSEUR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <QColorDialog>
#include <QColor>
#include <QPalette>
#include <opencv2/opencv.hpp>
#include <map>
#include <cstring>
#include <algorithm>
typedef unsigned char OCTET;
typedef unsigned char OCTET;

std::vector<float> RGBtoHSV(int ntaille,OCTET* ImgIn){
    std::vector<float> HSV;
    for (int i=0; i < ntaille*3; i+=3)
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
        H = std::round(H);
        HSV.push_back(H);
        HSV.push_back(S);
        HSV.push_back(cmax);
        // std::cout << "H = " <<  H <<  "S = " <<  S <<  "V = " <<  cmax << std::endl;

    }
    return HSV;
}

std::vector<int> getHistoHSV(std::vector<float> ImgIn, int nTaille3){

    std::vector<int> histogramme(nTaille3, 0);

    for(int i = 0; i < nTaille3; i+=3){
        int valeur = ImgIn[i];
        histogramme[valeur]++;
    }

    return histogramme;

}

std::vector<int> getHistoRGB(OCTET* ImgIn, int nTaille3){

    std::vector<int> redHisto(256, 0);
    std::vector<int> greenHisto(256, 0);
    std::vector<int> blueHisto(256, 0);

    for(int i = 0; i < nTaille3; i+=3){


        int r = ImgIn[i];
        redHisto[r]++;

        int g = ImgIn[i+1];
        greenHisto[g]++;

        int b = ImgIn[i+2];
        blueHisto[b]++;
    }


    int redMax = 0;
    int greenMax = 0;
    int blueMax = 0;
    int redIndex = 0;
    int greenIndex = 0;
    int blueIndex = 0;

    for (int i = 0; i < 256; i++) {

        if (redHisto[i] > redMax) {
            redMax = redHisto[i];
            redIndex = i;
        }
        if (greenHisto[i] > greenMax) {
            greenMax = greenHisto[i];
            greenIndex = i;
        }
        if (blueHisto[i] > blueMax) {
            blueMax = blueHisto[i];
            blueIndex = i;
        }
    }

    return { redIndex, greenIndex, blueIndex };
}



int sommeVoisinHSV(const std::vector<int>& histoHSV, int teinte) {
    int somme = 0;
    int voisins = 5; // Nombre de voisins à inclure de chaque côté de la teinte

    if (teinte >= 0 && teinte < histoHSV.size()) {
        // Parcours des voisins dans la plage spécifiée
        for (int i = std::max(0, teinte - voisins); i <= std::min((int)histoHSV.size() - 1, teinte + voisins); ++i) {
            somme += histoHSV[i];
        }
    }

    return somme;
}

int teinte(const std::vector<int>& histoHSV){
    int result = 0;
    int value = 0;

    for(int i = 0; i < histoHSV.size(); i++){
        if (histoHSV[i] > value){
            value = histoHSV[i];
            result = i;
        }
    }
    return result;
}


void HSVtoRGB(float H, float S, float V, int *r, int *g, int *b) {
    float C = S * V;
    float X = C * (1 - fabs(fmod(H / 60.0, 2) - 1));
    float m = V - C;
    float Rs, Gs, Bs;

    if(H >= 0 && H < 60) {
        Rs = C; Gs = X; Bs = 0;
    } else if(H < 120) {
        Rs = X; Gs = C; Bs = 0;
    } else if(H < 180) {
        Rs = 0; Gs = C; Bs = X;
    } else if(H < 240) {
        Rs = 0; Gs = X; Bs = C;
    } else if(H < 300) {
        Rs = X; Gs = 0; Bs = C;
    } else {
        Rs = C; Gs = 0; Bs = X;
    }

    *r = (Rs + m) * 255;
    *g = (Gs + m) * 255;
    *b = (Bs + m) * 255;
}


float interpolationT(float teinte1, float teinte2, float f) {
    float distance = teinte2 - teinte1;
    if (distance > 180) {
        distance -= 360;
    } else if (distance < -180) {
        distance += 360;
    }
    return fmod((teinte1 + distance * f + 360), 360);
}

float teinteD(float h1, float h2) {
    float d = std::abs(h1 - h2);
    return fmin(d, 360 - d);
}


std::vector<float>findBestHarmonieMono(const std::vector<int>& histoHSV, std::vector<float> ImgIn, int nTaille3, QColor colorValue) {
    std::vector<float> ImgOut;
    ImgOut.resize(nTaille3);

    if(colorValue.hue() == 0 && colorValue.saturation() == 0 && colorValue.value() ==0){
        int t = teinte(histoHSV);
        std::cout << t << std::endl;
        for(int i = 0; i < nTaille3; i+=3){
            ImgOut[i] = t;
            ImgOut[i+1] = ImgIn[i+1];
            ImgOut[i+2] = ImgIn[i+2];
        }
    }else{
        for(int i = 0; i < nTaille3; i+=3){
            ImgOut[i] = colorValue.hue();
            ImgOut[i+1] = ImgIn[i+1];
            ImgOut[i+2] = ImgIn[i+2];
        }

    }



    return ImgOut;
}

void Moyenneur( std::vector<float>& imgIn, std::vector<float>& imgOut, int largeur, int hauteur) {
    for (int y = 1; y < hauteur-1; ++y) {
        for (int x = 1; x < largeur-1; ++x) {
            float sommeR = 0, sommeG = 0, sommeB = 0;
            int count = 0;
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    int ny = y + i;
                    int nx = x + j;
                    if (nx >= 0 && nx < largeur && ny >= 0 && ny < hauteur) {
                        int a = (ny * largeur + nx) * 3;
                        sommeR += imgIn[a];
                        sommeG += imgIn[a + 1];
                        sommeB += imgIn[a + 2];
                        count++;
                    }
                }
            }
            int i = (y * largeur + x) * 3;
            imgOut[i] = sommeR / count;
            imgOut[i + 1] = sommeG / count;
            imgOut[i + 2] = sommeB / count;
        }
    }
}

void contours(OCTET* imgIn, OCTET* imgOut, int h, int l) {
    cv::Mat image(h, l, CV_8UC3, imgIn);
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 1.5);
    cv::Mat contourImg;
    cv::Canny(gray, contourImg, 100, 200);
    cv::cvtColor(contourImg, contourImg, cv::COLOR_GRAY2BGR);
    memcpy(imgOut, contourImg.data, l * h * 3 * sizeof(OCTET));
}

void gauss(const std::vector<float>& in, std::vector<float>& out, int l, int h) {
    cv::Mat imageIn(h, l, CV_32FC3, const_cast<float*>(in.data()));
    cv::Mat imageOut;
    cv::GaussianBlur(imageIn, imageOut, cv::Size(5, 5), 2);
    memcpy(out.data(), imageOut.data, l * h * 3 * sizeof(float));
}
void gauss(OCTET* in, OCTET* out, int l, int h) {
    cv::Mat imageIn(h, l, CV_8UC3, in);
    cv::Mat imageOut;
    cv::GaussianBlur(imageIn,imageOut,cv::Size(5,5), 10);
    memcpy(out, imageOut.data, l * h * 3 * sizeof(OCTET));
}
std::vector<float> findBestHarmonieCompl(const std::vector<int>& histoHSV, std::vector<float> ImgIn, int nTaille3) {
    std::vector<float> ImgOut;
    ImgOut.resize(nTaille3);

    int t = teinte(histoHSV);
    int complementary_t = (t + 180) % 360; // Calculer la couleur complémentaire

    // Calculer la différence de teinte entre chaque pixel et la teinte dominante
    std::vector<int> delta_t(nTaille3 / 3);
    for (int i = 0; i < nTaille3; i += 3) {
        delta_t[i / 3] = std::abs(static_cast<int>(ImgIn[i]) - t);
    }

    // Utiliser la couleur dominante pour les pixels dont la différence de teinte est inférieure à 90,
    // et la couleur complémentaire sinon
    for (int i = 0; i < nTaille3; i += 3) {
        if (delta_t[i / 3] < 90) {
            ImgOut[i] = t; // Teinte dominante
        } else {
            ImgOut[i] = complementary_t; // Teinte complémentaire
        }
        ImgOut[i+1] = ImgIn[i+1];
        ImgOut[i+2] = ImgIn[i+2]; // Conserver la luminance
    }

    return ImgOut;
}
std::vector<float> choosedCompl(const std::vector<int>& histoHSV,std::vector<float> ImgIn, int nTaille3, QColor colorValue) {
    std::vector<float> ImgOut;
    ImgOut.resize(nTaille3);
    int t = 0;
    if(colorValue.hue() == 0 && colorValue.saturation() == 0 && colorValue.value() ==0){
        t = teinte(histoHSV);
    }else{
        t = colorValue.hue();
    }

    int tri1 = (t + 180) % 360;
    for(int i = 0 ; i < nTaille3; i+=3){
        float distT = teinteD(ImgIn[i], t);
        float distT1 = teinteD(ImgIn[i], tri1);
        float teinteInt;
        if (distT < distT1) {
            float T = 1.0f - (distT / 180.0f);
            T = fmax(T,0.7);
            teinteInt =interpolationT(ImgIn[i], distT, T);
        } else {
            float T1 = 1.0f - (distT1 / 180.0f);
            T1 = fmax(T1,0.7);
            teinteInt = interpolationT(ImgIn[i], distT1, T1);
        }
        ImgOut[i] = teinteInt;
        ImgOut[i + 1] = ImgIn[i + 1];
        ImgOut[i + 2] = ImgIn[i + 2];
    }

    return ImgOut;
}

std::vector<float> choosedHarmonieTri(const std::vector<int>& histoHSV, std::vector<float> ImgIn, int nTaille3, QColor colorValue) {
    std::vector<float> ImgOut;
    ImgOut.resize(nTaille3);

    int t = colorValue.isValid() ? colorValue.hue() : teinte(histoHSV);
    int tri1 = (t + 120) % 360;
    int tri2 = (t + 240) % 360;



    for(int i = 0 ; i < nTaille3; i+=3){
        float teinte = ImgIn[i];
        float distT = teinteD(teinte, t);
        float distT1 = teinteD(teinte, tri1);
        float distT2 = teinteD(teinte, tri2);

        float proche = t;
        float minDistance = distT;
        if (distT1 < minDistance) {
            minDistance = distT1;
            proche = tri1;
        }
        if (distT2 < minDistance) {
            minDistance = distT2;
            proche = tri2;
        }

        float interpolation = 1.0f - (minDistance / 120.0f);
        interpolation = fmax(interpolation,0.7);
        ImgOut[i] = interpolationT(teinte, proche, interpolation);
        ImgOut[i + 1] = ImgIn[i + 1];
        ImgOut[i + 2] = ImgIn[i + 2];
    }

    return ImgOut;
}

std::vector<float> choosedHarmonieAnalogue(const std::vector<int>& histoHSV, std::vector<float> ImgIn, int nTaille3, QColor colorValue) {
    std::vector<float> ImgOut;
    ImgOut.resize(nTaille3);

    int t = colorValue.isValid() ? colorValue.hue() : teinte(histoHSV);

    int analog1 = (t + 30) % 360;
    int analog2 = (t - 30 + 360) % 360;


    for(int i = 0; i < nTaille3; i += 3) {
        float teinte = ImgIn[i];
        float distT = teinteD(teinte, t);
        float distT1 = teinteD(teinte, analog1);
        float distT2 = teinteD(teinte, analog2);
        float proche = t;
        float minDistance = distT;
        if (distT1 < minDistance) {
            minDistance = distT1;
            proche = analog1;
        }
        if (distT2 < minDistance) {
            minDistance = distT2;
            proche = analog2;
        }
        float interpolation = 1.0f - (minDistance / 300.0f);
        interpolation = fmax(interpolation,0.7);
        ImgOut[i] = interpolationT(teinte, proche, interpolation);
        ImgOut[i + 1] = ImgIn[i + 1];
        ImgOut[i + 2] = ImgIn[i + 2];
    }

    return ImgOut;
}



std::vector<float> findBestHarmonieTri(const std::vector<int>& histoHSV, std::vector<float> ImgIn, int nTaille3) {
    std::vector<float> ImgOut;
    ImgOut.resize(nTaille3);

    int t = teinte(histoHSV);
    // t = 39;
    int tri1 = (t + 120) % 360;
    int tri2 = (t + 240) % 360;

    for(int i = 0 ; i < nTaille3; i+=3){
        float distT = std::abs(static_cast<int>(ImgIn[i]) - t);
        float distT1 = std::abs(static_cast<int>(ImgIn[i]) - tri1);
        float distT2 = std::abs(static_cast<int>(ImgIn[i]) - tri2);


        int selected_triad;
        if (distT < distT1 && distT < distT2) {
            selected_triad = t;
        } else if (distT1 < distT && distT1 < distT2) {
            selected_triad = tri1;
        } else {
            selected_triad = tri2;
        }

        ImgOut[i] = selected_triad;
        ImgOut[i + 1] = 0.5;
        ImgOut[i + 2] = ImgIn[i + 2];

    }



    return ImgOut;
}

std::vector<float> findBestHarmonieAnalogue(const std::vector<int>& histoHSV, std::vector<float> ImgIn, int nTaille3) {
    std::vector<float> ImgOut;
    ImgOut.resize(nTaille3);

    int t = teinte(histoHSV);

    int analog1 = (t + 30) % 360;
    int analog2 = (t + 330) % 360;

    for(int i = 0 ; i < nTaille3; i+=3){
        int delta_t1 = std::abs(static_cast<int>(ImgIn[i]) - analog1);
        int delta_t2 = std::abs(static_cast<int>(ImgIn[i]) - analog2);

        int selected_analog;
        if (delta_t1 < delta_t2) {
            selected_analog = analog1;
        } else {
            selected_analog = analog2;
        }

        // Remplacer les valeurs de pixel par la teinte analogue sélectionnée
        ImgOut[i] = selected_analog;
        ImgOut[i + 1] = 0.5; // Saturation à 0.5
        ImgOut[i + 2] = ImgIn[i + 2]; // Conserver la luminance

    }


    return ImgOut;
}


#endif // CONVERTISSEUR_H
