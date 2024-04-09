#include <stdio.h>
#include "image_ppm.h"
#include <vector>
#include <iostream>
#include <algorithm> // std::min_element
#include <iterator>  // std::begin, std::end


int pixel_plus_petit_voisin(unsigned char *ImgIn, int i, int j, int nH, int nW) {
    int min = 255;
    //on parcours notre élément structurant (carré 3 x 3 ) et on cherche notre min sur notre cordonnée i, j actuelle
    for (int dx = -1; dx <= 1; dx++) { // on fais -1 0 1 en parcours, utile pour nos colonnes
        for (int dy = -1; dy <= 1; dy++) { //on fait -1 0 1 pour nos ligne
            //valeur du pixel de notre élment stucturant

            if ((i + dx) >= 0 && (i + dx) < nW && (j + dy) >= 0 && (j + dy) < nH) { // si on est dans notre borne
                if (ImgIn[(i + dx) * nW + (j + dy)] < min) {
                    min = ImgIn[(i + dx) * nW + (j + dy)];
                }
            }
        }
    }

    return min;
}

//récupère la valeur max de l'élément structurant (carré 3 x 3) depuis la cordonnée i, j, utilisés pour la dilatation
int pixel_plus_grand_voisin(unsigned char *ImgIn, int i, int j, int nH, int nW) {
    int max = 0;
    //on parcours notre élément structurant (carré 3 x 3 ) et on cherche notre min sur notre cordonnée i, j actuelle
    for (int dx = -1; dx <= 1; dx++) { // on fais -1 0 1 en parcours, utile pour nos colonnes
        for (int dy = -1; dy <= 1; dy++) { //on fait -1 0 1 pour nos ligne
            //valeur du pixel de notre élment stucturant

            if ((i + dx) >= 0 && (i + dx) < nW && (j + dy) >= 0 && (j + dy) < nH) { // si on est dans notre borne
                if (ImgIn[(i + dx) * nW + (j + dy)] > max) {
                    max = ImgIn[(i + dx) * nW + (j + dy)];
                }
            }
        }
    }

    return max;
}


int main(int argc, char *argv[]) {
    //la strat ici , c'est de récupérer dans 3 tableau différents "l'image" des composantes RGB puis sur chaque composante on va passer notre élément strurant pour
    //récupérer la valeur min
    char cNomImgLue[250];
    int nH, nW, nTaille;
    if (argc != 2) {
        printf("Usage: ImageIn.ppm \n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
   

    OCTET *ImgIn, *ImgOut, *ImgOut2;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW; // si pas carré faire (long * larg )/2

    allocation_tableau(ImgIn, OCTET, nTaille * 3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille * 3);
    allocation_tableau(ImgOut2, OCTET, nTaille * 3);


    OCTET *tab_red, *tab_green, *tab_blue; //définition pour nos tableaux contenant toutes nos pixel de composantes

    allocation_tableau(tab_red, OCTET, nTaille); // on défini nos espace mémoire pour nos tableaux
    allocation_tableau(tab_green, OCTET, nTaille);
    allocation_tableau(tab_blue, OCTET, nTaille);

    //on récupère nos valeur de componsante avec Plan, qui nous donne tout les pixel de composantes et les met dans un tableau
    planR(tab_red, ImgIn, nTaille);
    planV(tab_green, ImgIn, nTaille);
    planB(tab_blue, ImgIn, nTaille);

    OCTET *tab_val_min_red, *tab_val_min_green, *tab_val_min_blue;//tableu qui va contenir pour chaque case la valeur min de sa composante associée

    allocation_tableau(tab_val_min_red, OCTET, nTaille); //on défini nos espaces mméoire pour nos tableau
    allocation_tableau(tab_val_min_green, OCTET, nTaille);
    allocation_tableau(tab_val_min_blue, OCTET, nTaille);

    //maintenant on parcours nos tableau rouge vert bleu , et pour chaque composante , on récupère les valeurs des voisins la plus petite et on affecte le pixel
    //actuel de notre parcour avec la valeur min pour le rouge vert et bleu
    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            //recupérer valeur voisin min de la case [i,j] pour le tableau red , green et blue
            tab_val_min_red[i * nW + j] = pixel_plus_grand_voisin(tab_red, i, j, nH, nW);
            tab_val_min_green[i * nW + j] = pixel_plus_grand_voisin(tab_green, i, j, nH, nW);
            tab_val_min_blue[i * nW + j] = pixel_plus_grand_voisin(tab_blue, i, j, nH, nW);
        }

    }
    //nos tableau tab_val_min contiennet pour chaque case , la valeur min de l'élément structurant

    //on va mettre chaque valeur dans nos composante de notre image finale
    // on parcours chaque case de nos tableau de couleur de taille nTaille
    // vu que l'on met à jour chaque case de composante de ImngOut, on multiplie par 3 l'indice pour avoir la case correspondante pour la case i , i+1,i+2 de chaque couleur des tableau
    for (int i = 0; i < nTaille; i++) {
        ImgOut[i * 3] = tab_val_min_red[i];
        ImgOut[(i * 3) + 1] = tab_val_min_green[i];
        ImgOut[(i * 3) + 2] = tab_val_min_blue[i];
    }

    // Erosion
    OCTET *tab_red2, *tab_green2, *tab_blue2; //définition pour nos tableaux contenant toutes nos pixel de composantes

    allocation_tableau(tab_red2, OCTET, nTaille); // on défini nos espace mémoire pour nos tableaux
    allocation_tableau(tab_green2, OCTET, nTaille);
    allocation_tableau(tab_blue2, OCTET, nTaille);

    //on récupère nos valeur de componsante avec Plan, qui nous donne tout les pixel de composantes et les met dans un tableau
    planR(tab_red2, ImgOut, nTaille);
    planV(tab_green2, ImgOut, nTaille);
    planB(tab_blue2, ImgOut, nTaille);

    OCTET *tab_val_min_red2, *tab_val_min_green2, *tab_val_min_blue2;//tableu qui va contenir pour chaque case la valeur min de sa composante associée

    allocation_tableau(tab_val_min_red2, OCTET, nTaille); //on défini nos espaces mméoire pour nos tableau
    allocation_tableau(tab_val_min_green2, OCTET, nTaille);
    allocation_tableau(tab_val_min_blue2, OCTET, nTaille);

    //maintenant on parcours nos tableau rouge vert bleu , et pour chaque composante , on récupère les valeurs des voisins la plus petite et on affecte le pixel
    //actuel de notre parcour avec la valeur min pour le rouge vert et bleu
    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            //recupérer valeur voisin min de la case [i,j] pour le tableau red , green et blue
            tab_val_min_red2[i * nW + j] = pixel_plus_petit_voisin(tab_red2, i, j, nH, nW);
            tab_val_min_green2[i * nW + j] = pixel_plus_petit_voisin(tab_green2, i, j, nH, nW);
            tab_val_min_blue2[i * nW + j] = pixel_plus_petit_voisin(tab_blue2, i, j, nH, nW);
        }

    }
    //nos tableau tab_val_min contiennet pour chaque case , la valeur min de l'élément structurant

    //on va mettre chaque valeur dans nos composante de notre image finale
    // on parcours chaque case de nos tableau de couleur de taille nTaille
    // vu que l'on met à jour chaque case de composante de ImngOut, on multiplie par 3 l'indice pour avoir la case correspondante pour la case i , i+1,i+2 de chaque couleur des tableau
    for (int i = 0; i < nTaille; i++) {
        ImgOut2[i * 3] = tab_val_min_red2[i];
        ImgOut2[(i * 3) + 1] = tab_val_min_green2[i];
        ImgOut2[(i * 3) + 2] = tab_val_min_blue2[i];
    }

    ecrire_image_ppm((char *) "fermeture.ppm", ImgOut2, nH, nW);
    

}
