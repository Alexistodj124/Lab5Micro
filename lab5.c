#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_KIOSKOS 6
#define NUM_AUTOS_COMPASS 10 // Numero de carros Compass
#define NUM_AUTOS_EFECTIVO 10 // Numero de carros efe

// Variables
int tiempoTotalCompass = 0;
int tiempoTotalEfectivo = 0;
pthread_mutex_t mutexCompass;
pthread_mutex_t mutexEfectivo;

// Funcion para simular Compass
void pagoCompass() {
    printf("Kiosko Compass: Pago Compass realizado, vehículo puede pasar.\n");
    tiempoTotalCompass += 1; // 1 segundo pago en Compass
}

// Funcion para simular efe
void pagoEfectivo() {
    printf("Kiosko Efectivo: Esperando pago en efectivo...\n");
    sleep(2);
    printf("Kiosko Efectivo: Pago en efectivo realizado, vehículo puede pasar.\n");
    tiempoTotalEfectivo += 2; //segundos pago en efe
}

// Funcion para procesar pagos
void *procesarKiosko(void *arg) {
    int kiosko_id = *((int *)arg);
    
    while (1) {
        if (kiosko_id < NUM_KIOSKOS / 2 && tiempoTotalCompass < NUM_AUTOS_COMPASS) {
            pthread_mutex_lock(&mutexCompass);
            if (tiempoTotalCompass < NUM_AUTOS_COMPASS) {
                pagoCompass();
                pthread_mutex_unlock(&mutexCompass);
            } else {
                pthread_mutex_unlock(&mutexCompass);
                break;
            }
        } else {
            pthread_mutex_lock(&mutexEfectivo);
            if (tiempoTotalEfectivo < NUM_AUTOS_EFECTIVO) {
                pagoEfectivo();
                pthread_mutex_unlock(&mutexEfectivo);
            } else {
                pthread_mutex_unlock(&mutexEfectivo);
                break;
            }
        }
    }
    
    return NULL;
}

int main() {
    printf("Simulando 6 kioskos de pago en una carretera...\n");

    pthread_mutex_init(&mutexCompass, NULL);
    pthread_mutex_init(&mutexEfectivo, NULL);
    
    pthread_t threads[NUM_KIOSKOS];
    int thread_args[NUM_KIOSKOS];
    
    for (int i = 0; i < NUM_KIOSKOS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, procesarKiosko, &thread_args[i]);
    }

    for (int i = 0; i < NUM_KIOSKOS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_mutex_destroy(&mutexCompass);
    pthread_mutex_destroy(&mutexEfectivo);

    printf("\nSimulación completada.\n");

    return 0;
}