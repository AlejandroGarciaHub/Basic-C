//
//  main.c
//  ReadData
//
//  Created by Miguel Garcia on 8/30/17.
//  Copyright © 2017 Miguel Garcia. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int nData,nFeatures, nCentroides;
double **datos, **centroides,**centroidesVacio;
static FILE *fp_data, *fp_centroides;

int change;

void readData(FILE *data, int Ndata,int NFeatures,double **datos);
void inicializacion(int argc, char *argv[]);
void asignacion(double **datos,double **centroides,int nData, int nFeatures, int nCentroides);
void recalculo(double **datos, double **centroides,int nData, int nFeatures, int nCentroides);


int main(argc, argv)
    int argc;
    char * argv[];
{
    
    if ((fp_data=fopen(argv[1], "r+"))==NULL) {
        fprintf(stderr, "No file datos %s\n",argv[1]);
        return 0;
    }
    if ((fp_data=fopen(argv[4], "r+"))==NULL) {
        fprintf(stderr, "No file centriodes %s\n",argv[4]);
        return 0;
    }
    
    //Comienza el tiempo
    inicializacion(argc,argv);

    fp_data=fopen(argv[1],"r");
    readData(fp_data, nData,nFeatures, datos);
    fclose(fp_data);

    fp_centroides=fopen(argv[4],"r");
    readData(fp_centroides, nCentroides, nFeatures, centroides);
    fclose(fp_centroides);
    

    
    for (int i=0; i<nCentroides; i++) {
        
        
        for (int j=0; j<nFeatures; j++) {
            printf("%lf ",centroides[i][j]);
        }
        printf("\n");
    }
    
    for (int i=0; i<nCentroides; i++) {
        
        printf("Vacios");
        for (int j=0; j<nFeatures; j++) {
            printf("%lf ",centroidesVacio[i][j]);
        }
        printf("\n");
    }

    
    asignacion(datos,centroides,nData,nFeatures,nCentroides);
    recalculo(datos, centroides, nData, nFeatures, nCentroides);
    
    do {
        asignacion(datos,centroidesVacio,nData,nFeatures,nCentroides);
        recalculo(datos, centroidesVacio, nData, nFeatures, nCentroides);
    }
    while(change==1);
    //Numero de iteraciones
    //Tiempo en milisegundos
    //Error de agrupamiento
    
    
    
    
//    printf("CAX %lf: ",cAX);
//    printf("CBX %lf: ",cBX);
//    printf("CCX %lf: ",cCX);
//    
//    printf("CAY %lf: ",cAY);
//    printf("CBY %lf: ",cBY);
//    printf("CCY %lf: ",cCY);

    return 0;
}

void readData(FILE *data, int Ndatos,int Nfeatures,double **datos){
    int i;
    int j;
    
    char palabra[30];
    
    
    for (i=0;i<Ndatos; i++) {
        for (j=0; j<Nfeatures; j++) {
            fscanf(data, "%s",palabra);
            datos[i][j]=atof(palabra);
        }
    }
}

void inicializacion(int argc, char *argv[]){
    

    nData=atoi(argv[2]);
    nFeatures=atoi(argv[3]);
    
    nCentroides=atoi(argv[5]);
    
    datos=(double **) malloc(nData*sizeof(double *));
    centroides=(double **) malloc(nCentroides*sizeof(double *));
    centroidesVacio=(double **) malloc(nCentroides*sizeof(double *));
    int i;
    for(i=0;i<nData;i++){
        datos[i]=(double *)malloc((nFeatures+3)*sizeof(double));
    }
    
    for(i=0;i<nCentroides;i++){
        centroides[i]=(double *)malloc(nFeatures*sizeof(double));
    }
    for(i=0;i<nCentroides;i++){
        centroidesVacio[i]=(double *)malloc(nFeatures*sizeof(double));
    }
   
}

void asignacion(double **datos,double **centroides,int nData, int nFeatures, int nCentroides){
    
    int i,j,k,group;

        for (i=0; i<nData; i++) {
            for (j=0; j<nFeatures-1; j++) {
                group=0;
                for (k=0; k<nCentroides; k++) {
                    double distancia=sqrt(pow(datos[i][j]-centroides[k][j], 2)+pow(datos[i][j+1]-centroides[k][j+1], 2));
                    if (k==0) {
                        
                        printf("Group 1");

                        datos[i][2]=distancia;
                        group=(k+1);
                    }
                    else if (distancia<datos[i][2]){
                        printf("Group 2");

                        datos[i][2]=distancia;
                        group=(k+1);
                    }
    //                printf(": %lf\n",distancia);
                    
                    printf("Distancia [%d",(i+1));
                    printf("] [%d",(j+1));
                    printf("] =%lf\n",distancia);
                    printf("Group: %d\n",group);
                }
                if (group!=0) {
                    datos[i][2]=group;
                }
                
            }
    }
    
    for (int i=0; i<nData; i++) {
        
        
        for (int j=0; j<nFeatures+1; j++) {
            printf("%lf ",datos[i][j]);
        }
        printf("\n");
    }
}

void recalculo(double **datos, double **centroides,int nData, int nFeatures, int nCentroides){
    
    int sum;
    int size;
    
    int i,j,k;
    for (k=1; k<=nCentroides; k++) {
        for (j=0; j<nFeatures; j++) {
            sum=0;
            size=0;
            change=0;
            for (i=0; i<nData; i++) {
                if ((int)datos[i][2]==k) {
                    sum+=datos[i][j];
                    size++;
                }
            }
            printf("Sum: %d\n",sum);
            printf("Size: %d\n",size);

            if ((double)centroidesVacio[k-1][j]!=((double)sum/(double)size)) {
                printf("Hubo cambio: %lf",centroidesVacio[k-1][j]);
                printf("Hubo cambio: %lf\n ",((double)sum/(double)size));
                change=1;
                centroidesVacio[k-1][j]=((double)sum/(double)size);
            }
        
        }
    }
    
    
    for (int i=0; i<nCentroides; i++) {
        
        printf("Vacios");
        for (int j=0; j<nFeatures; j++) {
            printf("%lf ",centroidesVacio[i][j]);
        }
        printf("\n");
    }
    printf("Cambio: %d\n",change);
    
}
