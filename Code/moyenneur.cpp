// test_couleur.cpp : Seuille une image en niveau de gris


#include <stdio.h>
#include "image_ppm.h"

int getIndice(int i, int j, int nW, int nH) {
    i = std::min(std::max(i, 0), nH - 1);
    j = std::min(std::max(j, 0), nW - 1);
    return i * nW + j;

}

int calculer_moyenne_voisin_gris_8(unsigned char *ImgIn, int i, int j, int nH, int nW) {
    return (ImgIn[getIndice(i, j, nH, nW)]
            + ImgIn[getIndice(i - 1, j, nH, nW)] //colonne -1
            + ImgIn[getIndice(i + 1, j, nH, nW)] // colonne +1
            + ImgIn[getIndice(i, j - 1, nH, nW)] // ligne -1
            + ImgIn[getIndice(i, j + 1, nH, nW)] // ligne -1
            + ImgIn[getIndice(i - 1, j - 1, nH, nW)] //ligne -1 colonne -1
            + ImgIn[getIndice(i - 1, j + 1, nH, nW)] //ligne -1 colonne +1
            + ImgIn[getIndice(i + 1, j + 1, nH, nW)] //ligne +1 colonne +1
            + ImgIn[getIndice(i + 1, j - 1, nH, nW)] //ligne -1 colonne +1
           ) / 9;
}


int main(int argc, char *argv[])
{

    char cNomImgLue[250];
    char cNomImgEcrite[250];
    int nH, nW, nTaille;

    if (argc != 2) {
        printf("Usage: ImageIn.ppm\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);

    OCTET *ImgIn, *ImgOut, *TabRedMoyenneVoisins, *TabGreenMoyenneVoisins, *TabBlueMoyenneVoisins, *TabRed, *TabGreen, *TabBlue;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille * 3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille * 3);

    allocation_tableau(TabRed, OCTET, nTaille);
    allocation_tableau(TabGreen, OCTET, nTaille);
    allocation_tableau(TabBlue, OCTET, nTaille);
    allocation_tableau(TabRedMoyenneVoisins, OCTET, nTaille);
    allocation_tableau(TabGreenMoyenneVoisins, OCTET, nTaille);
    allocation_tableau(TabBlueMoyenneVoisins, OCTET, nTaille);

    planR(TabRed, ImgIn, nTaille);
    planV(TabGreen, ImgIn, nTaille);
    planB(TabBlue, ImgIn, nTaille);

    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            TabRedMoyenneVoisins[getIndice(i, j, nW, nH)] = calculer_moyenne_voisin_gris_8(TabRed, i, j, nH, nW);
            TabGreenMoyenneVoisins[getIndice(i, j, nW, nH)] = calculer_moyenne_voisin_gris_8(TabGreen, i, j, nH, nW);
            TabBlueMoyenneVoisins[getIndice(i, j, nW, nH)] = calculer_moyenne_voisin_gris_8(TabBlue, i, j, nH, nW);

        }
    }

    for (int i = 0; i < nTaille; i++) {
        ImgOut[i * 3] = TabRedMoyenneVoisins[i];
        ImgOut[(i * 3) + 1] = TabGreenMoyenneVoisins[i];
        ImgOut[(i * 3) + 2] = TabBlueMoyenneVoisins[i];
    }

    ecrire_image_ppm((char *) "moyenneur.ppm", ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    free(TabRed);
    free(TabGreen);
    free(TabBlue);
    free(TabRedMoyenneVoisins);
    free(TabBlueMoyenneVoisins);
    free(TabGreenMoyenneVoisins);
}