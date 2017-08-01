#include <time.h>
#include <stdio.h>             /* for convenience */
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>

typedef struct _control{
    int init;
    int end;
    long *arr;
} control;

int aleatorio(int a, int b){
    return (rand()%(b-a+1))+a;
}

control *numeroHilo(int a,int b){
    int di,modulo,acum=0;
    control *arreglo=(control *)malloc (sizeof(control)*b);   
    modulo=a%b;
    if(modulo == 0){
    //escogeremos el entero menor de la division
        di=a/b;
        
        for(int i=0;i<b;i++)
        {
            arreglo[i].init=acum;
            acum=acum+(di-1);
            arreglo[i].end=acum;
            acum=acum+1;
        }  
    }else{
        //cogemos el entero mayor
        //cuando la division no es exacta
        di=(a/b);
        int n=a-1;
        for(int i=0;i<b;i++)
       {
            if(n>=acum)
            {
                arreglo[i].init=acum;
                acum=acum+(di-1);
                arreglo[i].end=acum;
                acum=acum+1;
            }
            //coge el resto de los caracteres
            else{
                arreglo[i].init=acum;
                acum=a-1;
                arreglo[i].end=acum;
            }
            n=n-di;
       }  
    }
    return arreglo;
}

double obtenerTiempoActual(){
    struct timespec tsp;

    clock_gettime( CLOCK_REALTIME, &tsp);

    double secs = (double)tsp.tv_sec;
    double nano = (double)tsp.tv_nsec/1000000000.0;

    return secs+nano;
}

void *sumador(void *ctrl) {
    int i,acum=0;
    control *data = (control *)ctrl;
    
    for(i=data->init;i<data->end;i++){
        acum=acum+data->arr[i];
    }
   return (void *)acum;
}



int main(int argc, char *argv[]){

    pthread_t thr[atoi(argv[2])];
    int i,rc;
    long sumatotal;
    long *arr;
    arr= (long*) malloc(sizeof(long)*atoi(argv[1]));
    if (argc==1) {
		printf("./Taller11 <tamaño_arreglo> <numero_hilos>");
		exit(1);
	}
    
    srand(time(0));
    double ti = obtenerTiempoActual();
    for(i=0;i<atoi(argv[1]);i++){
        arr[i]=aleatorio(50,200);
    }
    control *arreglo=numeroHilo(atoi(argv[1]),atoi(argv[2]));
    for (i = 0; i < atoi(argv[2]); i++) {
        arreglo[i].arr=arr;
        if ((rc = pthread_create(&thr[i], NULL, sumador, &arreglo[i]))) {
            fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
            return EXIT_FAILURE;
        }
    }
    void *sumaparcial=NULL;
    for (i = 0; i < atoi(argv[2]); ++i) {
        pthread_join(thr[i], &sumaparcial);
        printf("Valor Hilo %d: %lu\n",(i+1),(long)sumaparcial);
        sumatotal =sumatotal+ (long)sumaparcial;
    }
    double tf = obtenerTiempoActual()-ti;
    printf("Suma Total: %d\n",(int)sumatotal);

	printf("Tiempo de ejecucion: %9f segundos\n",tf);
    return 0;
}