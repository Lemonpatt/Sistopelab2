#include "fbroker.h"

//test
void create_broker(int pipe_fd[2], int cantidad_workers) {
    if (fork() == 0) {
        
        int envio = dup(pipe_fd[1]);
        close(pipe_fd[1]);
        int lectura = dup(pipe_fd[0]); // Redirigir stdin para leer desde el pipe
        close(pipe_fd[0]);

        //Pasamos la cantidad de trabajadores a un string para pasarlo por execl
        char cantidad_workers_str[100];
        snprintf(cantidad_workers_str, sizeof(cantidad_workers_str), "%d", cantidad_workers);

        char num_envio[100];
        snprintf(num_envio, sizeof(num_envio), "%d", envio);
        char num_lectura[100];
        snprintf(num_lectura, sizeof(num_lectura), "%d", lectura);


        execl("./broker", "broker", cantidad_workers_str, num_envio, num_lectura, NULL); // Ejecutar el worker
        perror("execl");
        exit(1);
    }
}

// Función para reconstruir la imagen BMP a partir de los resultados de los workers
void reconstruct_image(BMPImage* original_image, BMPImage** results, int num_workers) {
    int columna_actual = 0;
    // Recorrer cada worker
    for (int w = 0; w < num_workers; w++) {
        int start_column = columna_actual;
        int end_column = columna_actual + results[w]->width;
        // Copiar los datos procesados de cada worker a la imagen original
        for (int col = 0; col < results[w]->width; col++) {
            for (int y = 0; y < original_image->height; y++) {
                original_image->data[y * original_image->width + start_column + col] = results[w]->data[y * original_image->width + col];
            }
        }
        columna_actual = end_column;
    }
}