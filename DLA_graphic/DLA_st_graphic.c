#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

// Dimensione grid
#define GRID_SIZE 500

// Dimesione "influenza" del cristallo
#define CLUSTER_RADIUS 1

// Valori che possono assumere le particelle
#define BACKGROUND 0
#define GROWN_CRYSTAL 1

// Numero particella da immettere nel sistema 
int NUM_PARTICLES;

// Creo una matrice quadrata  
int grid[GRID_SIZE][GRID_SIZE];

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

// Inizializzo e dichiaro una finestra e un renderer SDL
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


// Funzione per controllare se una posizione è valida sulla griglia
int isValid(int x, int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

// Funzione per verificare se una particella ha raggiunto il cristallo
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

    int x,y;
    int direction;
    // La particella ottiene la sua posizione iniziale in modo random
    x = rand() % GRID_SIZE;
    y = rand() % GRID_SIZE;

    while (1) {
        // Controlla se la particella in movimento si trova vicino al cristallo     
        if (isTouchingCluster(x, y))
        {
            // (true) La particella diventa cristallo cresciuto
            grid[x][y] = GROWN_CRYSTAL;
            return NULL;
        }

        // (false) Movimento casuale della particella
        direction = rand() % 4; // 0: su, 1: giù, 2: sinistra, 3: destra
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


// Creo una funzione per renderizzare l'immagine finale
void render() {

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // colore Background 
    SDL_RenderClear(renderer);

    for (int i = 0; i < GRID_SIZE; i++) 
    {
        for (int j = 0; j < GRID_SIZE; j++) 
        {
            if (grid[i][j] == BACKGROUND) 
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Colore spazio vuoto 
            } 
            else 
            {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Colore seme
            } 

            SDL_Rect rect = {i, j, 1, 1};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    // Prende da terminale il numero di particelle
    NUM_PARTICLES = atoi(argv[1]);

    // Dichiaro inizio e fine per timer
    struct timespec start_time, end_time;

    // Inizializzo il generatore pseudo-randomico
    srand(time(NULL));

    printf("\nSimulazione in corso. Attendere ...\n");

    // Lancio la funzione di inizializzazione della grid a BACKGROUND
    init_grid();

    // Inizializzo il seme come punto centrale della grid
    int centerX = GRID_SIZE / 2;
    int centerY = GRID_SIZE / 2;
    grid[centerX][centerY] = GROWN_CRYSTAL;

    // Start del timer
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    
    for (int i = 0; i < NUM_PARTICLES; i++) 
    {
        simulate_particle();
    }

    // End del timer
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
                          
    printf("\nSimulazione creata con successo!\n");
    printf("Particelle totali: %d\n", NUM_PARTICLES);
    printf("Tempo impiegato: %f secondi\n", elapsed_time);

    // Controllo il lancio della libreria SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Inizializzo la finestra SDL
    window = SDL_CreateWindow("Diffusion-Limited Aggregation (ST)",
     SDL_WINDOWPOS_UNDEFINED,
     SDL_WINDOWPOS_UNDEFINED, 
     GRID_SIZE, 
     GRID_SIZE, 
     SDL_WINDOW_SHOWN);

    // Controllo che la finestra SDL sia stata creata correttamente
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Inizializzo il renderer SDL
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    // Renderizza il risultato finale
    render();

    // Aspetta che l'utente chiuda la finestra
    SDL_Event e;
    int quit = 0;
    while (!quit) 
    {
        while (SDL_PollEvent(&e) != 0) 
        {
            if (e.type == SDL_QUIT) 
            {
                quit = 1;
            }
        }
    }

    // Chiudo il render e la finestra SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    //Chiudo la libreria SDL
    SDL_Quit();

    return 0;
}
