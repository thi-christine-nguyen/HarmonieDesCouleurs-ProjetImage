
#include <stdio.h>
#include "image_ppm.h"
#include "math.h"
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}
int partition (int arr[], int b, int t) {
    int pivot = arr[t];    
    int i = (b - 1);  

    for (int j = b; j <= t- 1; j++) {
        if (arr[j] <= pivot) {
            i++;   
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[t]);
    return (i + 1);
}
void quickSort(int arr[], int b, int t) {
    if (b < t) {
        int pivot = partition(arr, b, t);
        quickSort(arr, b, pivot - 1);
        quickSort(arr, pivot + 1, t);
    }
}


int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;
  
  if (argc != 2) 
     {
       printf("Usage: ImageIn.ppm\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;

   OCTET *ImgIn;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   int nTaille3 = nTaille * 3;
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
	int mR,mG,mB,eR,eG,eB,maxR,maxG,maxB;
    int minR,minG,minB;
    mR =0; mG =0; mB =0; eR =0; eB =0; eG =0; maxR =0; maxG =0; maxB =0;
    minR = 255; minG = 255;  minB= 255; 
   // int tabR[nTaille];
    //int tabG[nTaille];
    //int tabB[nTaille];
    int *tabR = (int *)malloc(nTaille * sizeof(int));
    int *tabG = (int *)malloc(nTaille * sizeof(int));
    int *tabB = (int *)malloc(nTaille * sizeof(int));
    if (!tabR || !tabG || !tabB) {
    printf("Erreur d'allocation mémoire\n");
    free(tabR);
    free(tabG);
    free(tabB);
    exit(EXIT_FAILURE);
    }
    int j = 0;
   for (int i=0; i < nTaille3; i+=3)
     { mR += ImgIn[i];
       mG += ImgIn[i+1];
       mB += ImgIn[i+2];
       tabR[j]=ImgIn[i];
       tabG[j]=ImgIn[i+1];
       tabB[j]=ImgIn[i+2];
       j++;
       if(ImgIn[i]>maxR)
       maxR = ImgIn[i];
       else if (ImgIn[i]<minR)
       minR = ImgIn[i];
       if(ImgIn[i+1]>maxG)
       maxG = ImgIn[i+1];
       else if (ImgIn[i+1]<minG)
       minG = ImgIn[i+1];
       if(ImgIn[i+2]>maxB)
       maxB = ImgIn[i+2];
       else if (ImgIn[i+2]<minB)
       minB = ImgIn[i+2];
     }
     mR = mR/nTaille;
     mG = mG/nTaille;
     mB = mB/nTaille;
    quickSort(tabR, 0, nTaille-1);
    quickSort(tabG, 0, nTaille-1);
    quickSort(tabB, 0, nTaille-1);
     int medianeR = (tabR[nTaille/2]+tabR[(nTaille/2)+1])/2;
     int medianeG = (tabG[nTaille/2]+tabG[(nTaille/2)+1])/2;
     int medianeB = (tabB[nTaille/2]+tabB[(nTaille/2)+1])/2;
     for (int i=0; i < nTaille3; i+=3)
     {
        eR+=pow(ImgIn[i]-mR,2);
        eG+=pow(ImgIn[i+1]-mG,2);
        eB+=pow(ImgIn[i+2]-mB,2);
     }
     eR = sqrt(eR/nTaille);
     eG = sqrt(eG/nTaille);
     eB = sqrt(eB/nTaille);
    printf (" Moyennes R G B : %d   %d   %d \n",mR,mG,mB);
    printf (" ecart-types R G B : %d   %d   %d \n",eR,eG,eB);
    printf (" médianes R G B : %d   %d   %d \n",medianeR,medianeG,medianeB);
    printf (" max R G B : %d   %d   %d \n",maxR,maxG,maxB);
    printf (" min R G B : %d   %d   %d \n",minR,minG,minB);
   free(ImgIn);
   return 1;
}
