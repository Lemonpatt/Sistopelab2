#include "fworker.h"


void create_worker(int pipe_fdw[2], int pipe_fdb[2], int id_worker) {
    if (fork() == 0) {
        
        close(pipe_fdw[1]);
        close(pipe_fdb[0]);
        int envio = dup(pipe_fdb[1]);
        close(pipe_fdb[1]);
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

BMPImage* create_image_worker(int id_lectura) {

    BMPImage* image = (BMPImage*)malloc(sizeof(BMPImage));

    read(id_lectura, &image->width, sizeof(int));
    read(id_lectura, &image->height, sizeof(int));

    image->data = (RGBPixel*)malloc(sizeof(RGBPixel) * image->width * image->height);

    // Copy the relevant data from the original image to the section
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            RGBPixel pixel;
            read(id_lectura, &pixel.r, sizeof(unsigned char));
            read(id_lectura, &pixel.g, sizeof(unsigned char));
            read(id_lectura, &pixel.b, sizeof(unsigned char));
            image->data[y * image->width + x] = pixel;
        }
    }
    return image;
}


//Entradas: image, id_worker, cantidad_filtros, factor_saturacion, umbral_binarizacion, resultados_filtros[]
//Salidas: void
//Descripción: Función que aplica los filtros a una imagen BMP y guarda en un arreglo las imagenes obtenidas
void aplicar_filtros(BMPImage* image, int id_worker, int cantidad_filtros, double factor_saturacion, double umbral_binarizacion, BMPImage* resultados_filtros[]){


    //Aplicamos filtro de saturacion primero
    BMPImage* new_image = saturate_bmp(image, factor_saturacion);

    resultados_filtros[0] = new_image;
    write_bmp("worker_saturado.bmp", resultados_filtros[0]);

    //Revisamos si se hacen los siguientes filtros
    if (cantidad_filtros > 1){

        //Se aplica filtro de grises
        BMPImage* new_imageG = greyScale_bmp(image);
        resultados_filtros[1] = new_imageG;
        write_bmp("worker_gris.bmp", resultados_filtros[1]);

            if (cantidad_filtros == 3){

                //Otorgamos el filepath al nombre para escribirlo en la carpeta dada

                BMPImage* new_imageB = Binarizar_bmp(new_imageG, umbral_binarizacion);
                resultados_filtros[2] = new_imageB;
                write_bmp("worker_binario.bmp", resultados_filtros[2]);


            }


        }

        //Liberamos el espacio
        free_bmp(image);


}

void enviar_resultados(int id_envio, BMPImage* resultados_filtros[], int cantidad_filtros){

    for(int i = 0; i < cantidad_filtros; i++){

        for (int y = 0; y < resultados_filtros[i]->height; y++) {
            for (int x = 0; x < resultados_filtros[i]->width; x++) {
                RGBPixel pixel = resultados_filtros[i]->data[y * resultados_filtros[i]->width + x];
                write(id_envio, &pixel.r, sizeof(unsigned char));
                write(id_envio, &pixel.g, sizeof(unsigned char));
                write(id_envio, &pixel.b, sizeof(unsigned char));
                //printf("WORKER pixel %dx%d, r: %d g: %d, b: %d\n", x, y, pixel.r, pixel.g, pixel.b);
            }
        }
        int new_fd = dup(id_envio);
        close(id_envio);
        id_envio = new_fd;
        }
    close(id_envio);
}
