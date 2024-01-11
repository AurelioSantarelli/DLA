#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

FILE* file;
const char* log_file = "log_performance.txt";
const char* log_st = "log_st.txt";
const char* log_mt = "log_mt.txt";

int logCreate()
{
    // Creo un log dove andrò a salvare info utili
    file = fopen(log_file, "w");
    int logStatus;

    if (file == NULL) 
    {
        // Impossibile aprire il file
        perror("Errore nell'apertura del log_file");
    }
    

    return logStatus;

}
// Funzione che legge e salva sull'heap ogni riga del log file.
float* processLogFile(const char* filename, int* num_lines) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Determina dinamicamente il numero di righe nel file
    int lines = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
    }

    // Riporta il cursore all'inizio del file
    fseek(file, 0, SEEK_SET);

    float* elapsed_times = malloc(lines * sizeof(float));
    if (elapsed_times == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int line_count = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && line_count < lines) {
        float elapsed_time;
        
        sscanf(line, "%*s %*s %*d %*d:%*d:%*d %*d %*d,%*d,%f", &elapsed_time);

        elapsed_times[line_count] = elapsed_time;

        line_count++;
    }

    fclose(file);

    // Restituisci il numero di righe tramite il puntatore num_lines
    *num_lines = lines;

    return elapsed_times;
}

int main() {
    logCreate();
    printf("Confronto tempistiche DLA single-threaded e DLA-multithreaded con diversi numeri di particelle\n\n");
    printf("#############################################################################\n\n");
    fprintf(file, "Confronto tempistiche DLA single-threaded e DLA-multithreaded con diversi numeri di particelle\n\n");
    fprintf(file, "#############################################################################\n\n");

    int procs;
    int cnt = 0;
    int j = 0;
    int num_lines;
    float *elapsed_times_st = processLogFile(log_st, &num_lines);
    float *elapsed_times_mt = processLogFile(log_mt, &num_lines);
    float avg_speedup = 0;
    

    while(cnt < num_lines)
    {
        if (j == 0)
        {
            procs = 1000;
        }
        else
        {
            procs = j * 5000;
        }

        float elapsed_time_st = elapsed_times_st[cnt];
        float elapsed_time_mt = elapsed_times_mt[cnt];

        float speedup = elapsed_time_st / elapsed_time_mt;
        float eff = elapsed_time_st / (procs * elapsed_time_mt);
        printf("%d riga del log:\n\n",cnt+1);
        printf("(SINGLE-THREAD)\n #Particelle : %d\t|\tTempo impiegato : %f\n\n", procs, elapsed_time_st);
        printf("(MULTI-THREAD)\n #Particelle : %d\t|\tTempo impiegato : %f\n\n", procs, elapsed_time_mt);
        printf("SPEEDUP : %f\t\t\t\tEFFICENZA : %f\n\n", speedup, eff);
        printf("#############################################################################\n\n");
        fprintf(file, "%d riga del log:\n\n", cnt + 1);
        fprintf(file, "(SINGLE-THREAD)\n #Particelle : %d\t|\tTempo impiegato : %f\n\n", procs, elapsed_time_st);
        fprintf(file, "(MULTI-THREAD)\n #Particelle : %d\t|\tTempo impiegato : %f\n\n", procs, elapsed_time_mt);
        fprintf(file, "SPEEDUP : %f\t\t\t\tEFFICENZA : %f\n\n", speedup, eff);
        fprintf(file, "#############################################################################\n\n");

        j++;
        if(j == 4)
        {
            j = 0;
        }
        cnt++;
    }

    fclose(file);
    free(elapsed_times_st);
    free(elapsed_times_mt);

    return 0;
}
