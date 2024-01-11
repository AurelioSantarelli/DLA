#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdatomic.h>

#define creato 0
#define aggiornato 1

// Dimensione grid
#define GRID_SIZE 500

#define CLUSTER_RADIUS 1

// Valori che possono assumere le particelle
#define BACKGROUND 0
#define GROWN_CRYSTAL 1

int NUM_PARTICLES;
FILE* file;
const char* log_file = "log_mt.txt";

// Creo una matrice quadrata  
int grid[GRID_SIZE][GRID_SIZE];

// Inizializzo un mutex per thread
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Creo una funzione per ottenere valori random 
long getRandomNumber() 
{
    return rand();
}

// Inizializzo la grid al valore di default
void init_grid() 
{
    for (int i = 0; i < GRID_SIZE; i++) 
    {
        for (int j = 0; j < GRID_SIZE; j++) 
        {
            grid[i][j] = BACKGROUND;
        }
    }
}

// Funzione per controllare se una posizione è valida sulla griglia
int isValid(int x, int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

// Funzione per verificare se una particella ha raggiunto il cluster
int isTouchingCluster(int x, int y) {
    for (int i = -CLUSTER_RADIUS; i <= CLUSTER_RADIUS; i++) {
        for (int j = -CLUSTER_RADIUS; j <= CLUSTER_RADIUS; j++) {
            if (isValid(x + i, y + j) && grid[x + i][y + j] == GROWN_CRYSTAL) {
                return 1;
            }
        }
    }
    return 0;
}

// Creo una funzione che simula una particella nella grid
void* simulate_particle() 
{

    int x, y;
    int direction;

    x = getRandomNumber() % GRID_SIZE;
    y = getRandomNumber() % GRID_SIZE;

    while (1)
    {       
        
        if (isTouchingCluster(x, y))
        {
            // La particella diventa cristallo cresciuto
            /* pthread_mutex_lock(&mutex);
            grid[x][y] = GROWN_CRYSTAL;
            pthread_mutex_unlock(&mutex); */

            atomic_store(&grid[x][y], GROWN_CRYSTAL);
            
            return NULL;
        }

        direction = getRandomNumber() % 4; // 0: su, 1: giù, 2: sinistra, 3: destra
        switch (direction)
        {
        case 0:
            y--;
            break;
        case 1:
            y++;
            break;
        case 2:
            x--;
            break;
        case 3:
            x++;
            break;
        }

        // Assicurati che la posizione sia valida
        x = (x + GRID_SIZE) % GRID_SIZE;
        y = (y + GRID_SIZE) % GRID_SIZE;

    }
    
}


void getTime()
{
    struct timeval tv;
    
    // Ottieni il timestamp corrente
    if (gettimeofday(&tv, NULL) != 0) {
        perror("Errore nell'ottenere il timestamp");
    }

    // Estrai i secondi e i microsecondi
    time_t seconds = tv.tv_sec;
    suseconds_t microseconds = tv.tv_usec;

    // Ottieni la stringa dalla funzione ctime
    char* timeString = ctime(&seconds);

    // Rimuovi il carattere di nuova linea, se presente
    size_t length = strlen(timeString);
    if (length > 0 && timeString[length - 1] == '\n') {
        timeString[length - 1] = '\0';
    }

    // Stampa la data e l'ora con precisione al secondo e microsecondo
    fprintf(file,"%s %ld,", timeString, microseconds); 
}


int logCreate()
{
    // Creo un log dove andrò a salvare info utili
    file = fopen(log_file, "a+");
    int logStatus;

    if (file == NULL) {
        // Il file non esiste, prova ad aprirlo in modalità scrittura ("w")
        file = fopen(log_file, "w");
        logStatus = creato;
    }
    else
    {
        logStatus = aggiornato;
    }

    if (file == NULL) 
    {
        // Impossibile aprire il file
        perror("Errore nell'apertura del log_file");
    }
    
    getTime();

    return logStatus;

}

int main(int argc, char *argv[])
{
    NUM_PARTICLES = atoi(argv[1]);

    int logValue = logCreate();

    clock_t start_time, end_time;

    // Lancio la funzione di inizializzazione della grid a 0
    init_grid();

    // Setto il seme come punto centrale della grid
    int centerX = GRID_SIZE / 2;
    int centerY = GRID_SIZE / 2;
    grid[centerX][centerY] = GROWN_CRYSTAL;
  
    // Dichiaro #NUM_PARTICLES pthread
    pthread_t particles[NUM_PARTICLES];

    start_time = clock();

    // Crea threads per le particelle
    for (int i = 0; i < NUM_PARTICLES; i++) {
        pthread_create(&particles[i], NULL, simulate_particle,NULL);
    }

    end_time = clock();

    // Aspetto che ogni thread abbia finito l'esecuzione
    for (int i = 0; i < NUM_PARTICLES; i++) {
        pthread_join(particles[i], NULL);
    }

    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("\nSimulazione creata successo!\n");

    fprintf(file,"%d,%f\n", NUM_PARTICLES, elapsed_time);

    if(logValue == creato)
    {
        printf("%s creato con successo!\n", log_file);
    }
    else
    {
        printf("%s aggiornato con successo!\n", log_file);
    }

    fclose(file);

    return 0;
}
