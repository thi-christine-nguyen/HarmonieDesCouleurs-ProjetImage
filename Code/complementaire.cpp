#include <stdio.h>
#include "image_ppm.h"
#include "math.h"

int main(int argc, char* argv[]) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille;
    int h1, h2, s1, s2, v1, v2;

    if (argc != 9) {
        printf("Usage: ImageIn.ppm ImageOut.ppm H1 S1 V1 H2 S2 V2\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    sscanf(argv[3], "%d", &h1);
    sscanf(argv[4], "%d", &s1);
    sscanf(argv[5], "%d", &v1);
    sscanf(argv[6], "%d", &h2);
    sscanf(argv[7], "%d", &s2);
    sscanf(argv[8], "%d", &v2);

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille3);
    int moyenneH=0;
    int moyenneS=0;
    int moyenneV=0;
    for (int i = 0; i < nTaille3; i += 3) {
        if (i == 0 || i == nTaille3-3) continue; 
    ImgOut[i] = (ImgIn[i-3] + ImgIn[i] + ImgIn[i+3]) / 3;
    ImgOut[i+1] = (ImgIn[i-2] + ImgIn[i+1] + ImgIn[i+4]) / 3;
    ImgOut[i+2] = (ImgIn[i-1] + ImgIn[i+2] + ImgIn[i+5]) / 3;
    }

    for (int i = 0; i < nTaille3; i += 3){
        moyenneS+=ImgIn[i+1];
        moyenneV+=ImgIn[i+2];
    }
    for (int i = 0; i < nTaille3; i += 3) {
        int H = ImgIn[i];
        int S = ImgIn[i + 1];
        int V = ImgIn[i + 2];
        int distance1 = fmin(abs(H - h1), 360 - abs(H - h1));
        int distance2 = fmin(abs(H - h2), 360 - abs(H - h2));
        bool plus = (distance1 <= distance2) ? (H < h1 || H > h1 + 180) : (H < h2 || H > h2 + 180);
        int distance = fmin(distance1, distance2);
        float attenuation;
        if (distance < 30) {
            attenuation = 1.0;
        } else if (distance < 45) {
            attenuation = 0.95;
        } else if (distance < 60) {
            attenuation = 0.90;
        } else if (distance < 75) {
            attenuation = 0.85;
        } else if (distance < 90) {
            attenuation = 0.80;
        } else if (distance < 105) {
            attenuation = 0.75;
        } else if (distance < 120) {
            attenuation = 0.70;
        } else if (distance < 135) {
            attenuation = 0.65;
        } else {
            attenuation = 0.60;
        }
        // H
        if (plus) {
            H = (H + (int)(distance * attenuation)) % 360;
        } else {
            H = (H - (int)(distance * attenuation) + 360) % 360;
        }
        // S & V
        S = (distance == distance1) ? s1*(1-(S/s1)) : s2*(1-(S/s2)); 
        V = ((distance == distance1) ? v1*(1-(V/v1)) : v2*(1-(V/v2)));   
        /*if (V < ((distance == distance1) ? v1 : v2)) {
            V = ((distance == distance1) ? v1*(1-(V/v1)) : v2*(1-(V/v2)));
        } else if (V < 70) {
            V += 10; 
        } */

       
        float ditheringTest = (rand() % 100) / 500.0 - 0.1;
        H += ditheringTest * H; 
        S += ditheringTest * S; 
        V += ditheringTest * V; 

        ImgOut[i] = H;
        ImgOut[i + 1] = S;
        ImgOut[i + 2] = V;
    }

    ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);

    return 1;
}
