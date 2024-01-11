#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

const char* output = "output.txt";

void processFile(char* filename, int fileDescriptor) {
    FILE *file = fopen(filename, "r");
    int out_file_fd = fileDescriptor;

    // Inizializza le variabili somma e contatore per ciascun indice
    int somma_1000 = 0, somma_5000 = 0, somma_10000 = 0, somma_15000 = 0;
    int contatore_1000 = 0, contatore_5000 = 0, contatore_10000 = 0, contatore_15000 = 0;
    double sum_squared_diff_1000 = 0.0;
    double sum_squared_diff_5000 = 0.0;
    double sum_squared_diff_10000 = 0.0;
    double sum_squared_diff_15000 = 0.0;
    double media_1000 = 0, media_5000 = 0, media_10000 = 0, media_15000 = 0;
    double ds_1000 = 0, ds_5000 = 0, ds_10000 = 0, ds_15000 = 0;

    char line[MAX_LINE_LENGTH];
    double value;

    // Legge il file linea per linea
    while (fgets(line, sizeof(line), file) != NULL) {
        // Tokenizza la riga
        char *token = strtok(line, ",");
        int token_count = 0;

        // Ignora il primo token
        token = strtok(NULL, ",");

        // Analizza i token successivi
        while (token != NULL) {
            switch (token_count) {
                case 0:
                    // Secondo token
                    // Rimuovi spazi bianchi
                    while (*token == ' ') {
                        token++;
                    }

                    // Converti in intero solo se la parte iniziale della stringa è un numero
                    if (isdigit(*token) || (*token == '-' && isdigit(*(token + 1)))) {
                        if (atoi(token) == 1000) {
                            value = atof(strtok(NULL, ","));
                            sum_squared_diff_1000 += value * value;
                            somma_1000 += value;
                            contatore_1000++;
                        } else if (atoi(token) == 5000) {
                            value = atof(strtok(NULL, ","));
                            sum_squared_diff_5000 += value * value;
                            somma_5000 += value;
                            contatore_5000++;
                        } else if (atoi(token) == 10000) {
                            value = atof(strtok(NULL, ","));
                            sum_squared_diff_10000 += value * value;
                            somma_10000 += value;
                            contatore_10000++;
                        } else if (atoi(token) == 15000) {
                            value = atof(strtok(NULL, ","));
                            sum_squared_diff_15000 += value * value;
                            somma_15000 += value;
                            contatore_15000++;
                        }
                    }
                    break;

                default:
                    // Altri token, ignorati in questo esempio
                    strtok(NULL, ",");
                    break;
            }

            token = strtok(NULL, ",");
            token_count++;
        }
    }

    // Calcola le deviazioni standard solo se il conteggio è maggiore di zero
    if (contatore_1000 > 0) {
        media_1000 = somma_1000 / contatore_1000;
        ds_1000 = sqrt((sum_squared_diff_1000 / contatore_1000) - (media_1000 * media_1000));
    }

    if (contatore_5000 > 0) {
        media_5000 = somma_5000 / contatore_5000;
        ds_5000 = sqrt((sum_squared_diff_5000 / contatore_5000) - (media_5000 * media_5000));
    }

    if (contatore_10000 > 0) {
        media_10000 = somma_10000 / contatore_10000;
        ds_10000 = sqrt((sum_squared_diff_10000 / contatore_10000) - (media_10000 * media_10000));
    }

    if (contatore_15000 > 0) {
        media_15000 = somma_15000 / contatore_15000;
        ds_15000 = sqrt((sum_squared_diff_15000 / contatore_15000) - (media_15000 * media_15000));
    }

    // Stampa i risultati
    dprintf(out_file_fd, "Indice 1000: Media = %f, Deviazione standard = %f\n", media_1000, ds_1000);
    dprintf(out_file_fd, "Indice 5000: Media = %f, Deviazione standard = %f\n", media_5000, ds_5000);
    dprintf(out_file_fd, "Indice 10000: Media = %f, Deviazione standard = %f\n", media_10000, ds_10000);
    dprintf(out_file_fd, "Indice 15000: Media = %f, Deviazione standard = %f\n\n", media_15000, ds_15000);

    fclose(file);
}

int main() {
    FILE *out_file = fopen(output, "w");

    if (out_file == NULL) {
        perror("Errore nell'apertura del file");
        return 1;
    }
    int out_file_fd = fileno(out_file);

    fprintf(out_file,"Risultati single-threaded\n\n");
    fflush(out_file);
    processFile("log_st.txt",out_file_fd);
    fflush(out_file);
    fprintf(out_file,"Risultati multi-threaded\n\n");
    fflush(out_file);
    processFile("log_mt.txt",out_file_fd);
    fflush(out_file);

    fclose(out_file);
    return 0;
}
