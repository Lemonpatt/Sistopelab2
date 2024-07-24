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

BMPImage* get_worker_image_section(BMPImage* image, int id_worker, int cantidad_workers) {
    int total_columns = image->width;
    int columns_per_worker = total_columns / cantidad_workers;
    int start_column = (id_worker-1) * columns_per_worker;
    int end_column = (id_worker == cantidad_workers) ? total_columns : start_column + columns_per_worker;

    int num_columns = end_column - start_column;


    BMPImage* new_image = (BMPImage*)malloc(sizeof(BMPImage));
    new_image->width = image->width;
    new_image->height = image->height;
    new_image->data[image->width * image->height];

    printf("DEBE SER LA MISMA WIDTH PERO PASADA A COLUMNAS: %d \n", num_columns);
    // Copy the relevant data from the original image to the section
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            RGBPixel pixel = image->data[y * image->width + x];
            unsigned char pixelGrey = pixel.r * 0.3 + pixel.g * 0.59 + pixel.b* 0.11;
            pixel.r = (unsigned char)(pixelGrey);
            pixel.g = (unsigned char)(pixelGrey);
            pixel.b = (unsigned char)(pixelGrey);
            new_image->data[y * image->width + x] = pixel;

        }

    }
    printf("Start column: %d, End column: %d\n", start_column, end_column);
    return new_image;
}