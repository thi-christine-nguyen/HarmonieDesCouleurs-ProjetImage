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



std::vector<float>findBestHarmonieMono(const std::vector<int>& histoHSV, std::vector<float> ImgIn, int nTaille3, QColor colorValue) {
    std::vector<float> ImgOut;
    ImgOut.resize(nTaille3);

    if(colorValue.hue() == 0 && colorValue.saturation() == 0 && colorValue.value() ==0){
        int t = teinte(histoHSV);
        for(int i = 0; i < nTaille3; i+=3){
            ImgOut[i] = t;
            ImgOut[i+1] = 0.5;
            ImgOut[i+2] = ImgIn[i+2];
        }
    }else{
        for(int i = 0; i < nTaille3; i+=3){
            ImgOut[i] = colorValue.hue();
            ImgOut[i+1] = 0.5;
            ImgOut[i+2] = ImgIn[i+2];
        }

    }



    return ImgOut;
}
void Moyenneur( OCTET* imgIn, OCTET* imgOut, int largeur, int hauteur) {
    for (int y= 0; y< hauteur; ++y) {
        for (int x= 0; x< largeur; ++x) {
            int sommeR = 0,sommeG = 0, sommeB = 0,count = 0;
            for (int i = -1; i<= 1; ++i) {
                for (int j = -1; j<= 1; ++j) {
                    int ny =y+i;
                    int nx = x+j;
                    if (nx >= 0 && nx < largeur && ny >= 0 && ny < hauteur) {
                        int a = (ny*largeur + nx) * 3;
                        sommeR += imgIn[a];
                        sommeG += imgIn[a+1];
                        sommeB += imgIn[a+2];
                        count++;
                    }
                }
            }
            int i = (y * largeur + x) * 3;
            imgOut[i]  = sommeR / count;
            imgOut[i+1] = sommeG / count;
            imgOut[i+2] = sommeB / count;
        }
    }
}
struct Vec3bComparer {
    bool operator()(const cv::Vec3b& a, const cv::Vec3b& b) const {
        if (a[0]!=b[0])
            return a[0] < b[0];
        if (a[1]!=b[1])
            return a[1] < b[1];
        return a[2] < b[2];
    }
};

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstring>

typedef unsigned char OCTET;

void contours(OCTET* imgIn, OCTET* imgOut, int h, int l) {
    cv::Mat image(h, l, CV_8UC3, imgIn);
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 1.5);
    cv::Mat contours;
    double s1 = 100;
    double    s2 = 200;
    cv::Canny(gray, contours, s1, s2);
    cv::Mat contourImageRGB;
    cv::cvtColor(contours, contourImageRGB, cv::COLOR_GRAY2BGR);
    std::memcpy(imgOut, contourImageRGB.data, l * h *3 *sizeof(OCTET));
}
void appliquerLissageGaussien(OCTET* in, OCTET* out, int l, int h) {
    cv::Mat imageIn(h, l, CV_8UC3, in);
    cv::Mat imageOut;
    cv::GaussianBlur(imageIn,imageOut,cv::Size(3,3), 2);
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
        ImgOut[i+1] = 0.5;
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
        float distT = std::abs(ImgIn[i] - t);
        float distT1 = std::abs(ImgIn[i] - tri1);
        float teinteInt;
        if (distT < distT1) {
            float factorT = std::min(distT / 90.0f, 1.0f);
            teinteInt = t + factorT * ((t + 180) % 360 - t);
        } else {
            float factorT1 = std::min(distT1 / 90.0f, 1.0f);
            teinteInt = tri1 + factorT1 * ((tri1 + 180) % 360 - tri1);
        }
        ImgOut[i] = teinteInt;
        ImgOut[i + 1] = 0.5;
        ImgOut[i + 2] = ImgIn[i + 2];
    }

    return ImgOut;
}

std::vector<float> choosedHarmonieTri(const std::vector<int>& histoHSV,std::vector<float> ImgIn, int nTaille3, QColor colorValue) {
    std::vector<float> ImgOut;
    ImgOut.resize(nTaille3);

    int t = 0;


    if(colorValue.hue() == 0 && colorValue.saturation() == 0 && colorValue.value() ==0){
        t = teinte(histoHSV);
    }else{
        t = colorValue.hue();
    }

    int tri1 = (t + 120) % 360;
    int tri2 = (t + 240) % 360;

    for(int i = 0 ; i < nTaille3; i+=3){
        float distT = std::abs(ImgIn[i] - t);
        float distT1 = std::abs(ImgIn[i] - tri1);
        float distT2 = std::abs(ImgIn[i] - tri2);
        float factorT = std::min(distT / 90.0f, 1.0f);
        float teinteInt;
        if (distT < distT1 && distT < distT2) {
            teinteInt = t + factorT * ((t + 180) % 360 - t);
        } else if (distT1 < distT && distT1 < distT2) {
            factorT = std::min(distT1 / 90.0f, 1.0f);
            teinteInt = tri1 + factorT * ((tri1 + 180) % 360 - tri1);
        } else {
            factorT = std::min(distT2 / 90.0f, 1.0f);
            teinteInt = tri2 + factorT * ((tri2 + 180) % 360 - tri2);
        }

        ImgOut[i] = teinteInt;
        ImgOut[i + 1] = 0.5;
        ImgOut[i + 2] = ImgIn[i + 2];
    }

    return ImgOut;
}
std::vector<float> choosedHarmonieAnalogue(const std::vector<int>& histoHSV,std::vector<float> ImgIn, int nTaille3, QColor colorValue) {
    std::vector<float> ImgOut;
    ImgOut.resize(nTaille3);


    int t = 0;


    if(colorValue.hue() == 0 && colorValue.saturation() == 0 && colorValue.value() ==0){
        t = teinte(histoHSV);
    }else{
        t = colorValue.hue();
    }

    int analog1 = (t + 30) % 360;
    int analog2 = (t + 330) % 360;

    for(int i = 0 ; i < nTaille3; i+=3){
        float delta_t1 = std::abs(ImgIn[i] - analog1);
        float delta_t2 = std::abs(ImgIn[i] - analog2);

        float factorT1 = std::min(delta_t1 / 45.0f, 1.0f);
        float factorT2 = std::min(delta_t2 / 45.0f, 1.0f);

        float teinteInt;
        if (delta_t1 < delta_t2) {
            teinteInt = analog1 + factorT1 * ((analog1 + 180) % 360 - analog1);
        } else {
            teinteInt = analog2 + factorT2 * ((analog2 + 180) % 360 - analog2);
        }

        ImgOut[i] = teinteInt;
        ImgOut[i + 1] = 0.5;
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
