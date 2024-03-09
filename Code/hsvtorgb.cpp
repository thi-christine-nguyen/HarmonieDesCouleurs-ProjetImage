// test_couleur.cpp : Seuille une image couleur 

#include <stdio.h>
#include "image_ppm.h"
#include "math.h"


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


int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn.ppm ImageOut.ppm\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   int nTaille3 = nTaille * 3;
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille3);
    for (int i = 0; i < nTaille3; i += 3) {
        int R, G, B;
        float H = ImgIn[i]; 
        float S = ImgIn[i+1] / 100.0;
        float V = ImgIn[i+2] / 100.0;
        HSVtoRGB(H, S, V, &R, &G, &B);
        ImgOut[i] = R;
        ImgOut[i+1] = G;
        ImgOut[i+2] = B;
    }

   ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}
