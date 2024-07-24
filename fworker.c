#include "fworker.h"


void create_worker(int pipe_fdw[2], int id_worker) {
    if (fork() == 0) {

        int envio = dup(pipe_fdw[1]);
        close(pipe_fdw[1]);
        int lectura = dup(pipe_fdw[0]); // Redirigir stdin para leer desde el pipe
        close(pipe_fdw[0]);

        char id_worker_str[100];
        snprintf(id_worker_str, sizeof(id_worker_str), "%d", id_worker);

        char num_envio[100];
        snprintf(num_envio, sizeof(num_envio), "%d", envio);
        char num_lectura[100];
        snprintf(num_lectura, sizeof(num_lectura), "%d", lectura);

        execl("./worker", "worker", id_worker_str, num_envio, num_lectura, NULL); // Ejecutar el worker
        perror("execl");
        exit(1);
    }
}

RGBPixel* get_worker_image_section(BMPImage* image, RGBPixel* section_data, int id_worker, int cantidad_workers) {
    int total_columns = image->width;
    int columns_per_worker = total_columns / cantidad_workers;
    int start_column = id_worker-1 * columns_per_worker;
    int end_column = (id_worker == cantidad_workers) ? total_columns : start_column + columns_per_worker;

    int num_columns = end_column - start_column;

    if (!section_data) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Copy the relevant data from the original image to the section
    for(int col = start_column; col < end_column; col++){
        for (int row = 0; row < image->height; row++) {
            section_data[row * (col-start_column)] = image->data[(row + start_column) * col];
        }
    }
    return section_data;
}