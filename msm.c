#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

int ncajas;	//numero de cajas
int nclientes;		//numero de clientes
bool *cajas;		//cajas, true si esta ocupada, false si esta libre
int *espera_clientes;		//tiempo de espera de cada cliente
pthread_mutex_t mutx;		//mutex que controla la cola
pthread_cond_t caja_libre;	//condicion de que se ha dejado la caja libre

bool cajasOcupadas(){	//devuelve true si todas las cajas estan ocupadas
	int i;
	bool resultado;
	resultado = true;
	for (i = 0; i<ncajas; i++){
		resultado = resultado && cajas[i];
	}
	return resultado;
}

int cajaLibre(){	//devuelve una caja libre
	int i;
	int resultado;
	resultado = -1;
	for (i = 0; i<ncajas; i++){
		if (!cajas[i]){
			resultado = i;
		}
	}
	return resultado;
}

void *cliente(void * num){
	int cliente_id = *(int *)num;	//identificador del cliente
	int tini;	//tiempo de inicio de espera
	int tfin;	//tiempo de fin de espera
	int caja;	//caja que se le ajudica

	srand(time(0));		//crea una semilla
	printf("Cliente %d realizando compra.\n", cliente_id);
	sleep((rand() % 2) + 1);	//espera mientras compra
	tini = clock();
	
	printf("Cliente %d en la cola.\n", cliente_id);
	pthread_mutex_lock(&mutx);	//bloquea el mutex
	while (cajasOcupadas()){
		pthread_cond_wait(&caja_libre,&mutx);	//espera a que alguna caja este libre
	}
	
	//Inicio sección crítica.
	caja = cajaLibre();	//le asignamos una caja libre
	tfin = clock();
	printf("Cliente %d atendido en la caja %d.\n", cliente_id, caja);
	cajas[caja] = true;	//marcamos la caja como ocupada
	pthread_mutex_unlock(&mutx);	//libera el mutex
	//Fin sección crítica.
	sleep( (rand() % 2) + 1);	//espera mientras es atendido
	espera_clientes[cliente_id] = tfin-tini;
	
	//Inicio sección crítica.
	pthread_mutex_lock(&mutx);	//bloquea el mutex
	cajas[caja] = false;	//marcamos la caja como libre
	pthread_cond_signal(&caja_libre);	//mandamos la señal de que una caja está libre
	pthread_mutex_unlock(&mutx);	//libera el mutex
	//Fin sección crítica
	
	printf("Cliente %d saliendo despues de esperar %d .\n", cliente_id, tfin-tini);

	return 0;
}

int main (int argc, char *argv[]){

	int i;		//contador
	int *cliente_id;	//array dinamico de identificadores para los clientes
	pthread_t *th;		//array de threads que actuaran como clientes
	float media;
	float desviacion;
	float aux;

	if (argc==2){		//si solo nos pasan el numero de cajas
		ncajas=atoi(argv[1]);
		nclientes=20*ncajas;
	}
	else if (argc==3){	//si nos pasan el numero de cajas y el numero de clientes
		ncajas=atoi(argv[1]);
		nclientes=atoi(argv[2]);
	}	
	else{		//si no nos pasan argumentos
		printf("Error. Llamar al programa de la forma: %s CAJAS [CLIENTES].\n", argv[0]);
		return -1;
	}
	//reservamos memoria
	cajas = malloc(ncajas*sizeof(bool));
	cliente_id = malloc(nclientes*sizeof(int));
	th = malloc(nclientes*sizeof(pthread_t));
	espera_clientes = malloc(nclientes*sizeof(int));
	
	pthread_mutex_init(&mutx,NULL);		//iniciamos el mutex, la cola
	for (i = 0; i<ncajas; i++){	//inicializamos el array de cajas, todas libres
		cajas[i] = false;
	}
	pthread_cond_init(&caja_libre,NULL);	//inicializamos la condicion
	for(i = 0; i < nclientes; i++) {	//creamos los clientes, threads
		cliente_id[i] = i;
		pthread_create(&(th[i]),NULL,cliente,(void*)&(cliente_id[i]));	
	}
	for (i = 0; i<nclientes; i++){		//sincronizamos los clientes
		pthread_join(th[i],NULL);
	}
	pthread_mutex_destroy(&mutx);	//destruimos el mutex, la cola
	media = 0;
	for (i = 0; i<nclientes; i++){		//calculamos la media
		media = media + espera_clientes[i];
	}
	media = media/nclientes;
	desviacion = 0;
	for (i = 0; i<nclientes; i++){		//calculamos la desviacion tipica
		aux = (espera_clientes[i]-media)*(espera_clientes[i]-media);
		desviacion = desviacion + aux;
	}
	desviacion = desviacion/8;
	desviacion = sqrt(desviacion);
	
	//liberamos memoria
	free(cajas);
	free(cliente_id);
	free(th);
	free(espera_clientes);
	
	printf("Supermercado cerrado.\n");
	printf("Tiempo medio de espera: %f.\n", media);
	printf("Desviación típica del tiempo medio de espera: %f.\n", desviacion);
	return 0;
}
