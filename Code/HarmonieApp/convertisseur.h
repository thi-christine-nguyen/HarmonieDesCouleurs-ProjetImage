#ifndef CONVERTISSEUR_H
#define CONVERTISSEUR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>


std::vector<int> getHistoHSV(OCTET *ImgIn, int nTaille3){

    std::vector<int> histogramme(256, 0);

    // for (int i=0; i < nTaille3; i+=3){
    //     int value = ImgIn[i];
    //     std::cout << value << std::endl;
    //     histogramme[value]++;
    // }

    for(int i = 0; i < nTaille3; i+=3){
        int valeur = ImgIn[i];
        histogramme[valeur]++;
    }

    return histogramme;

}

int sommeVoisinHSV(const std::vector<int>& histoHSV, int teinte) {
    int somme = 0;
    int voisins = 5; // Nombre de voisins à inclure de chaque côté de la teinte

    // Assurez-vous que la teinte est dans la plage valide
    if (teinte >= 0 && teinte < histoHSV.size()) {
        // Parcours des voisins dans la plage spécifiée
        for (int i = std::max(0, teinte - voisins); i <= std::min((int)histoHSV.size() - 1, teinte + voisins); ++i) {
            somme += histoHSV[i];
        }
    }

    return somme;
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


OCTET* findBestHarmonieCompl(const std::vector<int>& histoHSV, OCTET *ImgIn, int nTaille3) {
    OCTET *ImgOut;

    allocation_tableau(ImgOut, OCTET, nTaille3);

    std::pair<int, int> mode = std::make_pair(0, 0);
    for (int color = 0; color < histoHSV.size(); color++) {
        int somme = sommeVoisinHSV(histoHSV, color);

        if (somme > mode.second) {
            mode = std::make_pair(color, somme);
        }
    }

    // On harmonise les couleurs de l'image
    for(int i = 0; i < nTaille3; i+=3){
        ImgOut[i] = mode.first;
        ImgOut[i+1] = ImgIn[i+1];
        ImgOut[i+2] = ImgIn[i+2];
    }


    return ImgOut;
}








#endif // CONVERTISSEUR_H
