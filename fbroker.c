#include "fbroker.h"

//test
void create_worker(int pipe_fd[2]) {
    if (fork() == 0) {
        close(pipe_fd[1]); // Cerrar el extremo de escritura del pipe

        dup2(pipe_fd[0], STDIN_FILENO); // Redirigir stdin para leer desde el pipe
        close(pipe_fd[0]);

        execl("./worker", "worker", NULL); // Ejecutar el worker
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