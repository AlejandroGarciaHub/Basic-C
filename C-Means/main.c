/*********************************************************************************************************************************************************
 
 Algoritmo k-means estándar implementado por Wilebaldo Bernal Flores
 
 Entrada: argv[1] = Número de objetos
 argv[2] = Número de centroides
 argv[3] = Número de características
 argv[4] = Nombre del archivo de los datos de entrada
 argv[5] = Nombre del archivo de los centroides de entrada
 argv[6] = Nombre del archivo donde se guardará el tiempo, el error al cuadrado y el número de iteraciones que hubo en la ejecución
 
 Salida: Archivo generado donde se guarda el tiempo medido en milisegundos, el error al cuadrado y el número de iteraciones hasta llegar a convergencia
 
 **********************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

void inicializar(int filas, int columnas, FILE *nombreArch, double **matriz);
void distanciaEuclidiana(int Nobjetos, int Ncentros, int Ncaracteristicas, double **centroides, double **datos);
void recalcularCentroides(int Nobjetos, int Ncentros, int Ncaracteristicas, double **datos, double **centroides, double **nuevosCentroides);
void centroidesNew(int Ncentros, int Ncaracteristicas, double **centroides, double **nuevosCentroides);
int condicionConvergencia(int Ncentros, int Ncaracteristicas, double **centroides, double **nuevosCentroides);
double calculoErrorCuadrado(int Nobjetos, int Ncentros, int Ncaracteristicas, double **centroides, double **datos);

double timeval_diff(struct timeval *a, struct timeval *b);

int main(int argc, char *argv[]){
    
    int i,Nobjetos,Ncentros,Ncaracteristicas;
    char *rutaDatos = (char *)malloc(sizeof(char)*20);
    char *rutaCentroides = (char *)malloc(sizeof(char)*20);
    char *rutaResult = (char *)malloc(sizeof(char)*20);
    
    if(argc == 7){ //Argumentos desde teclado
        Nobjetos = atoi(argv[1]); //Numero de objetos
        Ncentros = atoi(argv[2]); //Numero de centroides
        Ncaracteristicas = atoi(argv[3]); //Caracteristicas
        rutaDatos = argv[4];
        rutaCentroides = argv[5];
        rutaResult = argv[6];
    }
    
    FILE *archObj, *archCentros, *archResul;
    archObj = fopen(rutaDatos,"r"); //Abre el archivo datos.txt en modo lectura
    
    double **datos; //Reserva en la memoria una matriz de nombre "datos"
    datos = (double **) malloc(Nobjetos * sizeof(double *));
    for(i = 0; i < Nobjetos; i++)
        datos[i] = (double *) malloc((Ncaracteristicas+1) * sizeof(double));
    
    double **centroides; //Reserva en la memoria una matriz de nombre "centroides"
    centroides = (double **) malloc(Ncentros * sizeof(double *));
    for(i = 0; i < Ncentros; i++)
        centroides[i] = (double *) malloc(Ncaracteristicas * sizeof(double));
    
    double **nuevosCentroides; //Reserva en la memoria una matriz de nombre "nuevosCentroides"
    nuevosCentroides = (double **) malloc(Ncentros * sizeof(double *));
    for(i = 0; i < Ncentros; i++)
        nuevosCentroides[i] = (double *) malloc(Ncaracteristicas * sizeof(double));
    
    double **minMax; //Reserva en la memoria una matriz de nombre "minMax"
    minMax = (double **) malloc(Ncaracteristicas * sizeof(double *));
    for(i = 0; i < Ncaracteristicas; i++)
        minMax[i] = (double *) malloc(Ncaracteristicas * sizeof(double));
    
    inicializar(Nobjetos,Ncaracteristicas,archObj,datos); //Manda llamar la funcion inicializar para imprimir los valores del archivo de datos.txt
    fclose(archObj); //Cerrar el archivo datos.txt
    
    archCentros = fopen(rutaCentroides,"r"); //Abre el archivo centroides.txt en modo lectura
    inicializar(Ncentros,Ncaracteristicas,archCentros,centroides); //Imprime los primeros centroides
    
    int var = 0, iter = 0;
    
    struct timeval t_ini, t_fin;
    double secs, tiempo;
    /*******Inicia tiempo*******/
    gettimeofday(&t_ini, NULL);
    /***************************/
    
    while(var == 0 && iter!=100){
        //printf("\n");
        distanciaEuclidiana(Nobjetos,Ncentros,Ncaracteristicas,centroides,datos); //Calcula la distancia eucladiana para todos los centroides
        //printf("\n");
        recalcularCentroides(Nobjetos,Ncentros,Ncaracteristicas,datos,centroides,nuevosCentroides); //Recalcula los nuevos centroides y los imprime en pantalla
        var = condicionConvergencia(Ncentros,Ncaracteristicas,centroides,nuevosCentroides); //Determina si hay un paro de convergencia, y si lo hay arroja un 1 en caso contrario un 0
        centroidesNew(Ncentros,Ncaracteristicas,centroides,nuevosCentroides);
        iter++;
    }
    /*******Termina tiempo*******/
    gettimeofday(&t_fin, NULL);
    /****************************/
    secs = timeval_diff(&t_fin, &t_ini);
    
    double error = 0;
    error = calculoErrorCuadrado(Nobjetos,Ncentros,Ncaracteristicas,centroides,datos);
    
    tiempo = (secs * 1000.0); //Tiempo en milisegundos
    
    for (int i=0; i<Ncentros; i++) {
        
        printf("Vacios");
        for (int j=0; j<Ncaracteristicas; j++) {
            printf("%lf ",nuevosCentroides[i][j]);
        }
        printf("\n");
    }
    
    archResul = fopen(rutaResult,"a");
    fprintf(archResul,"%.16g\t %f\t %d\n", tiempo, error, iter); //Imprime en el archivo [Tiempo Error Iteracion]
    
    printf("\nError al cuadrado: %f\n", error);
    printf("Numero de iteraciones: %d\n", iter);
    printf("%.16g milliseconds\n", tiempo); //Imprime en pantalla el tiempo de ejecucion total desde inicio hasta fin en segundos
    
    for(i=0;i<Nobjetos;i++)
        free(datos[i]);
    free(datos);
    for(i=0;i<Ncentros;i++)
        free(centroides[i]);
    free(centroides);
    for(i=0;i<Ncentros;i++)
        free(nuevosCentroides[i]);
    free(nuevosCentroides);
    fclose(archCentros); //closing all the input files
    fclose(archResul);
    
    
    return 0;
}

/**********************************************************************************************************************
 ****************************************************** FUNCIONES ******************************************************
 ***********************************************************************************************************************/

/*************************************************************************************
 Inicializar: lee los datos desde un archivo y los inicializa en una matriz
 
 Entrada: filas = Número de filas que se utilizan ya sea objetos o centroides
 columnas = Número de características de la experimentación
 nombreArch = Nombre del archivo donde se van a leer los datos
 matriz = Arreglo donde se guardarán los datos del archivo leído
 
 Salida: Arreglo "matriz" donde se guardan los datos del archivo que se lee
 *************************************************************************************/

void inicializar(int filas, int columnas, FILE *nombreArch, double **matriz){
    
    int i,j;
    char c[20]; //Arreglo de caracteres donde se guardara la matriz que ingreso por parametro
    
    for(i = 0; i < filas; i++){
        for (j = 0; j < columnas; j++){
            fscanf(nombreArch, "%s", c); //Lee la primera cadena en el archivo nombreArch hasta que encuentra un espacio
            //printf("| %s |\t", c); //Imprime por cadena de carateres lo que leyo del archivo
            matriz[i][j] = atof(c); //Convierte la cadena a tipo double con la funcion atof
        }
        //printf("\n");
    }
}

/********************************************************************************************************
 Distancia euclidiana: se calcula la distancia euclidiana entre los objetos y los centroides
 
 Entrada: Nobjetos = Número de objetos
 Ncentros = Número de centroides
 Ncaracteristicas = Número de características
 centroides = Arreglo donde se almacenan los centroides de la iteración actual
 datos = Arreglo donde se almacenan los objetos
 
 Salida: Se asigna el número de centroide al cual pertenece cada objeto en la matriz datos
 *********************************************************************************************************/

void distanciaEuclidiana(int Nobjetos, int Ncentros, int Ncaracteristicas, double **centroides, double **datos){
    
    int i,j,k;
    
    for (i = 0; i < Nobjetos; i++){ //Numero de objetos
        double minimo = 0; //Se inicializa el minimo aqui porque habra un numero minimo por cada centroide
        
        for (j = 0; j < Ncentros; j++){ //Numero de centroides
            double sum = 0,resultado = 0; //Se inicializan estas variables porque se hacen diferentes sumas por cada centroide
            
            for (k = 0; k < Ncaracteristicas; k++){ //Numero de caracteristicas
                sum+= pow( centroides[j][k] - datos[i][k] , 2); //Ciclo para calcular la distancia eucladiana antes de sacar la raiz cuadrada
            }
            resultado = sqrt(sum); //Saca la raiz cuadrada del resultado anterior
            
            if (j == 0){ //En la primera iteracion se pone como minimo el primer valor calculado
                minimo = resultado; //El primer resualtdo se le asigna a la variable minimo
                datos[i][Ncaracteristicas] = (double) j; //Se le asigna el centroide al cual pertenece el minimo en una tercera linea de la matriz datos
            }
            
            if(resultado < minimo){ //Si el nuevo resultado es menor al valor anterior de la variable minimo ahora minimo tomara el valor de la variable resultado
                minimo = resultado;
                datos[i][Ncaracteristicas] = (double) j; //Se asigna el centroide al cual pertenece en tercera fila de la matriz datos
            }
        }
    }
    //Imprime el centroide que pertenece cada objeto
    /*printf("Centroide al que pertenece cada objeto: \n");
     for(i = 0; i < Nobjetos; i++){
     printf("Objeto %d: | %f |",i+1 , datos[i][Ncaracteristicas]);
     printf("\n");
     }*/
}

/*******************************************************************************************************
 Recalcular centroides: se determinan nuevos centroides basados en la iteración anterior
 
 Entrada: Nobjetos = Número de objetos
 Ncentros = Número de centroides
 Ncaracteristicas = Número de características
 datos = Arreglo donde se almacenan los objetos
 centroides = Arreglo donde se almacenan los centroides de la iteración actual
 nuevosCentroides = Arreglo donde se van a almacenan los nuevos centroides
 
 Salida: Arreglo de "nuevosCentroides" donde se guardarán los valores del recálculo de centroides
 ********************************************************************************************************/

void recalcularCentroides(int Nobjetos, int Ncentros, int Ncaracteristicas, double **datos, double **centroides, double **nuevosCentroides){
    
    int i,j,k,l,x,y;
    double indice[1][Ncaracteristicas];
    
    for (i = 0; i < Ncentros; i++){ //Numero de centroides
        int cont = 0;
        double promedio = 0;
        
        //Inicializa un vector donde se guardaran los valores de la primera y segunda columna de la matriz datos
        for (x = 0; x < 1; x++){
            for (y = 0; y < Ncaracteristicas; y++){
                indice[x][y] = 0;
            }
        }
        
        for (j = 0; j < Nobjetos; j++){ //Numero de objetos
            
            if(datos[j][Ncaracteristicas] == i){ //Se posiciona en la columna donde se guardan los centroides de la matriz datos y selecciona el numero de centroide
                
                for (k = 0; k < Ncaracteristicas; k++){ //Numero de caracteristicas
                    indice[0][k] += datos[j][k]; //Suma todos los valores de x y y donde el centroide sea igual
                }
                cont++; //Se incrementa un contador para saber cuantas veces cayo cada centroide en todos los datos
            }
        }
        
        //Cuando mi contador es igual a 0, evita calcula el promedio
        if(cont == 0){
            for (j = 0; j < Ncaracteristicas; j++){
                nuevosCentroides[i][j] = centroides[i][j]; //Se pasan los valores de x y y de los centroides actuales a los nuevos centroides
            }
        }
        
        //Si no se cumple lo anterior, se calcula el promedio de x y y para formar los nuevos centroides
        else{
            for (l = 0; l < Ncaracteristicas; l++){
                promedio = indice[0][l] / cont;
                nuevosCentroides[i][l] = promedio;
            }
        }
    }
    
    //Imprime Nuevos Centroides
    /*printf("Nuevos Centroides: \n");
     for(i = 0; i < Ncentros; i++){
     for (j = 0; j < Ncaracteristicas; j++){
     printf("| %f |\t", nuevosCentroides[i][j]);
     //centroides[i][j] = nuevosCentroides[i][j]; //Los nuevos centroides pasan a ser los actuales, asi que en la matriz "centroides" ahora se encuentran los nuevos centroides
     }
     printf("\n");
     }*/
}

/*************************************************************************************************
 Nuevos centroides: pasa los nuevos centroides a la matriz de centroides de la iteración anterior
 
 Entrada: Ncentros = Número de centroides
 Ncaracteristicas = Número de características
 centroides = Arreglo donde se almacenan los centroides de la iteración actual
 nuevosCentroides = Arreglo donde se van a almacenan los nuevos centroides
 
 Salida: Nuevos valores para el arreglo de "centroides"
 **************************************************************************************************/

void centroidesNew(int Ncentros, int Ncaracteristicas, double **centroides, double **nuevosCentroides){
    
    int i,j;
    
    for(i = 0; i < Ncentros; i++){
        for (j = 0; j < Ncaracteristicas; j++){
            centroides[i][j] = nuevosCentroides[i][j];
        }
    }
}

/*************************************************************************************************
 Condición de convergencia: esta función pregunta si exitirá o no un paro de convergencia
 
 Entrada: Ncentros = Número de centroides
 Ncaracteristicas = Número de características
 centroides = Arreglo donde se almacenan los centroides de la iteración actual
 nuevosCentroides = Arreglo donde se van a almacenan los nuevos centroides
 
 Salida: Si los valores del arreglo "nuevosCentroides" es diferente al de "centroides" retorna 0
 y existe paro de convergencia, en caso contrario retorna 1 y continua la ejecución
 **************************************************************************************************/

int condicionConvergencia(int Ncentros, int Ncaracteristicas, double **centroides, double **nuevosCentroides){
    
    int i,j;
    
    for(i = 0; i < Ncentros; i++){
        for (j = 0; j < Ncaracteristicas; j++){
            if(nuevosCentroides[i][j] != centroides[i][j]){ //Si la matriz de los nuevos centroides en la posicion 0,0 es diferente a los centroides actuales (centroides) entra al if
                return 0;
            }
        }
    }
    return 1;
}

/*******************************************************************************************************
 Cálculo de error al cuadrado: calcula el error al cuadrado de la iteración acutal
 
 Entrada: Nobjetos = Número de objetos
 Ncentros = Número de centroides
 Ncaracteristicas = Número de características
 centroides = Arreglo donde se almacenan los centroides de la iteración actual
 datos = Arreglo donde se almacenan los objetos
 
 Salida: error = variable double donde se almacena el valor del error al cuadrado de la iteración actual
 ********************************************************************************************************/

double calculoErrorCuadrado(int Nobjetos, int Ncentros, int Ncaracteristicas, double **centroides, double **datos){
    
    int i,j,k;
    double sum, error = 0;
    
    for (i = 0; i < Ncentros; i++){ //Numero de objetos
        
        for (j = 0; j < Nobjetos; j++){ //Numero de centroides
            sum = 0;
            
            if(datos[j][Ncaracteristicas] == i){
                
                for (k = 0; k < Ncaracteristicas; k++){ //Numero de caracteristicas
                    sum+= pow( centroides[i][k] - datos[j][k] , 2);
                }
                error += sqrt(sum);
            }
        }
    }
    return error;
}

/* Retorna "a - b" en segundos */
double timeval_diff(struct timeval *a, struct timeval *b){
    return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}
