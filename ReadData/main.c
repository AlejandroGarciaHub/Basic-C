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
#include <sys/time.h>

int nData,nFeatures, nCentroides;
double **datos, **centroides,**centroidesVacio;
static FILE *fp_data, *fp_centroides;

//clock_t start;
//clock_t end;

struct timeval t0;
struct timeval t1;
float elapsed;

int change;
int iteracion;

void readData(FILE *data, int Ndata,int NFeatures,double **datos);
void inicializacion(int argc, char *argv[]);
void asignacion(double **datos,double **centroides,int nData, int nFeatures, int nCentroides);
void recalculo(double **datos, double **centroides,int nData, int nFeatures, int nCentroides);
float timedifference_msec(struct timeval t0, struct timeval t1);
void sumatoria_error(double **datos,int nData,double **centroides,int nCentroides, int nFeatures);


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
//    start = clock();
    iteracion=1;
    gettimeofday(&t0, 0);
    
    gettimeofday(&t1, 0);
    elapsed = timedifference_msec(t0, t1);
    printf("Inicio, %f milliseconds.\n", elapsed);
    
    inicializacion(argc,argv);

    fp_data=fopen(argv[1],"r");
    readData(fp_data, nData,nFeatures, datos);
    fclose(fp_data);

    fp_centroides=fopen(argv[4],"r");
    readData(fp_centroides, nCentroides, nFeatures, centroides);
    fclose(fp_centroides);
    

    
//    for (int i=0; i<nCentroides; i++) {
//        
//        
//        for (int j=0; j<nFeatures; j++) {
//            printf("%lf ",centroides[i][j]);
//        }
//        printf("\n");
//    }
//    
//    for (int i=0; i<nCentroides; i++) {
//        
//        printf("Vacios");
//        for (int j=0; j<nFeatures; j++) {
//            printf("%lf ",centroidesVacio[i][j]);
//        }
//        printf("\n");
//    }


    asignacion(datos,centroides,nData,nFeatures,nCentroides);
    recalculo(datos, centroides, nData, nFeatures, nCentroides);
    
    
//    gettimeofday(&t1, 0);
//    elapsed = timedifference_msec(t0, t1);
//    printf("Iteracion %d, %f milliseconds.\n",iteracion, elapsed);
    
//    end = clock();
//    printf("Tiempo: %f\n", (double)(end - start));
    
    do {
//        gettimeofday(&t1, 0);
//        elapsed = timedifference_msec(t0, t1);
        iteracion++;
//        printf("Iteracion %d, %f milliseconds.\n",iteracion, elapsed);
//        end = clock();
//        printf("Tiempo: %f\n", (double)(end - start));
        asignacion(datos,centroidesVacio,nData,nFeatures,nCentroides);
        recalculo(datos, centroidesVacio, nData, nFeatures, nCentroides);
    }
    while(change==1);
//    end = clock();
//    printf("Tiempo final: %f\n", (double)((end - start)));

    //Numero de iteraciones
    //Tiempo en milisegundos
    gettimeofday(&t1, 0);
    elapsed = timedifference_msec(t0, t1);
    printf("Iteracion Final %d, %f milliseconds.\n",iteracion, elapsed);
    //Error de agrupamiento
    sumatoria_error(datos,nData,centroidesVacio,nCentroides,nFeatures);
    
    
    
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
                datos[i][nFeatures]=pow(10, 8);
                for (k=0; k<nCentroides; k++) {
                    double distancia=sqrt(pow(datos[i][j]-centroides[k][j], 2)+pow(datos[i][j+1]-centroides[k][j+1], 2));
 
                    if (distancia<datos[i][nFeatures]){

                        datos[i][nFeatures]=distancia;
                        group=(k+1);
                    }
//                    printf("Distancia [%d][%d] =%lf\n",i,j,distancia);
//                    printf("Group: %d\n",group);
                }
                if (group!=0) {
             //       printf("Asigno: %d\n",group);
                    datos[i][nFeatures]=group;
//                    printf("datosTres [%d][%d]: %lf\n",i,j,datos[i][nFeatures]);
//                    printf("Termino una fila\n");
                }
                
            }
    }
    
//    for (int i=0; i<nData; i++) {
//        
//        
//        for (int j=0; j<nFeatures+1; j++) {
//            printf("%lf ",datos[i][j]);
//        }
//        printf("\n");
//    }
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
                if ((int)datos[i][nFeatures]==k) {
                    sum+=datos[i][j];
                    size++;
                }
            }
//            printf("Sum: %d\n",sum);
//            printf("Size: %d\n",size);

            if ((double)centroidesVacio[k-1][j]!=((double)sum/(double)size)) {
//                printf("Hubo cambio: %lf",centroidesVacio[k-1][j]);
//                printf("Hubo cambio: %lf\n ",((double)sum/(double)size));
                change=1;
                centroidesVacio[k-1][j]=((double)sum/(double)size);
            }
        
        }
    }
    
    
//    for (int i=0; i<nCentroides; i++) {
//        
//        printf("Vacios");
//        for (int j=0; j<nFeatures; j++) {
//            printf("%lf ",centroidesVacio[i][j]);
//        }
//        printf("\n");
//    }
//    printf("Cambio: %d\n",change);
    
}

void sumatoria_error(double **datos,int nData,double **centroides,int nCentroides, int nFeatures ){
    int i,j,k;
    double error=0;
    
    for (k=1; k<=nCentroides; k++) {
        for (i=0; i<nData; i++) {
            for (j=0; j<nFeatures-1; j++) {
                if ((int)datos[i][nFeatures]==k) {
                    double distancia=sqrt(pow(datos[i][j]-centroides[k-1][j], 2)+pow(datos[i][j+1]-centroides[k-1][j+1], 2));
                    error+=distancia;
                }
            }
        }
    }
    printf("Sumatoria error de agrupamiento: %lf\n",error/2);
}

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
