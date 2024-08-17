#include "fbroker.h"


void create_broker(int pipe_fdb[2], int pipe_fdp[2], int cantidad_workers) {
    if (fork() == 0) {
        close(pipe_fdb[1]);
        close(pipe_fdp[0]);
        int envio = dup(pipe_fdp[1]);
        close(pipe_fdp[1]);
        int lectura = dup(pipe_fdb[0]); // Redirigir stdin para leer desde el pipe
        close(pipe_fdb[0]);
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

void send_image(BMPImage* image, int id_envio) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            RGBPixel pixel = image->data[y * image->width + x];
            write(id_envio, &pixel.r, sizeof(unsigned char));
            write(id_envio, &pixel.g, sizeof(unsigned char));
            write(id_envio, &pixel.b, sizeof(unsigned char));
            //printf("WORKER pixel %dx%d, r: %d g: %d, b: %d\n", x, y, pixel.r, pixel.g, pixel.b);
        }
    }
}
