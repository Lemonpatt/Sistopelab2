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


//actualmente el id_worker no se está usando, 
void aplicar_filtros(BMPImage* image, int id_worker, int cantidad_filtros, double factor_saturacion, double umbral_binarizacion, BMPImage* resultados_filtros[]){
    //Otorgamos el filepath al nombre para escribirlo en la carpeta dada
        sprintf(nombre_imagen, "%s/%s_Saturated.bmp", nombre_carpeta, nombre_imagenes[i]);
        //Aplicamos filtro de saturacion primero
        BMPImage* new_image = saturate_bmp(image, factor_saturacion);
        write_bmp(nombre_imagen, new_image);

        //Se abre en modo de adicion el archivo csv
        csv = fopen(nombre_archivo_csv, "a");

        //Revisamos si se hacen los siguientes filtros
        if (cantidad_filtros > 1){
            //Otorgamos el filepath al nombre para escribirlo en la carpeta dada
            sprintf(nombre_imagen, "%s/%s_Gris.bmp", nombre_carpeta, nombre_imagenes[i]);
            //Se aplica filtro de grises
            BMPImage* new_imageG = greyScale_bmp(image);
            write_bmp(nombre_imagen, new_imageG);

            if (cantidad_filtros == 3){

                //Otorgamos el filepath al nombre para escribirlo en la carpeta dada
                sprintf(nombre_imagen, "%s/%s_Binario.bmp", nombre_carpeta, nombre_imagenes[i]);
                //Se binariza la imagen de grises
                BMPImage* new_imageB = Binarizar_bmp(new_imageG, umbral_binarizacion);
                write_bmp(nombre_imagen, new_imageB);

                //Si se llegó hasta aca, quiere decir que se debe aplicar la funcion nearly_black aqui
                resultadoNB = nearly_black(new_imageB, umbral_clasificacion);
                fprintf(csv, "%s-Binarizada; %d\n", nombre_imagenes[i], resultadoNB);
                //Cerramos csv para que no se realice nearly_black con las otras imagenes filtradas
                csv = NULL;
                //Liberamos el espacio
                free_bmp(new_imageB);
            }
            //Se ve nearly black solo si no se ha hecho todavia
            if (csv != NULL){
                resultadoNB = nearly_black(new_imageG, umbral_clasificacion);
                fprintf(csv, "%s-Gris; %d\n", nombre_imagenes[i], resultadoNB);
                csv = NULL;
            }
            //Liberamos el espacio
            free_bmp(new_imageG);

        }
        //Se ve nearly black solo si no se ha hecho todavia
        if (csv != NULL){
            resultadoNB = nearly_black(new_image, umbral_clasificacion);
            fprintf(csv, "%s-Saturada; %d\n", nombre_imagenes[i], resultadoNB);
            csv = NULL;
        }
        //Liberamos el espacio
        free_bmp(image);
        free_bmp(new_image);

        //Se aumenta el contador
        i++;

    }
}


/*
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

                //Liberamos el espacio
                free_bmp(new_imageB);
            }
            //Liberamos el espacio
            free_bmp(new_imageG);

        }

        //Liberamos el espacio
        free_bmp(image);
        free_bmp(new_image);


}

void enviar_resultados(int id_envio, BMPImage* resultados_filtros[], int cantidad_filtros){

    for(int i = 0; i < cantidad_filtros; i++){

        for (int y = 0; y < resultados_filtros[i]->height; y++) {
            for (int x = 0; x < resultados_filtros[i]->width; x++) {
                RGBPixel pixel = resultados_filtros[i]->data[y * resultados_filtros[i]->width + x];
                write(id_envio, &pixel.r, sizeof(unsigned char));
                write(id_envio, &pixel.g, sizeof(unsigned char));
                write(id_envio, &pixel.b, sizeof(unsigned char));
                printf("WORKER pixel %dx%d, r: %d g: %d, b: %d\n", x, y, pixel.r, pixel.g, pixel.b);
            }
        }
    int new_fd = dup(id_envio);
    close(id_envio);
    id_envio = new_fd;
    }
    close(id_envio);
}
*/