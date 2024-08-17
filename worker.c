#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fworker.h"

int main(int argc, char *argv[]) {
    
    int id_worker = atoi(argv[1]);
    int id_envio = atoi(argv[2]);
    int id_lectura = atoi(argv[3]);

    int cantidad_workers, cantidad_filtros;
    double factor_saturacion;
    double umbral_binarizacion;
    RGBPixel pixel;

    char nombre_imagen[400];
    
    read(id_lectura, &cantidad_workers, sizeof(int));
    read(id_lectura, &cantidad_filtros, sizeof(int));
    read(id_lectura, &factor_saturacion, sizeof(double));
    read(id_lectura, &umbral_binarizacion, sizeof(double));
    
    BMPImage* resultados_filtros[cantidad_filtros];

    BMPImage* image = create_image_worker(id_lectura);
    printf("image dimensions: %dx%d \n", image->width, image->height);

    close(id_lectura);

    aplicar_filtros(image,cantidad_filtros, factor_saturacion, umbral_binarizacion, resultados_filtros);


    enviar_resultados(id_envio, resultados_filtros, cantidad_filtros);
    close(id_envio);

    free_bmp(image);
/*
    
    char *nombre_carpeta = "test";
    make_folder(nombre_carpeta);

    sprintf(nombre_imagen, "%s/a_Saturated.bmp", nombre_carpeta);
        //Aplicamos filtro de saturacion primero
        BMPImage* new_image_filtro = saturate_bmp(new_image, factor_saturacion);
        write_bmp(nombre_imagen, new_image_filtro);
     ej
    columna inicial 0
    n colum 10
    columna final = columna inicial + n colum - 1 = 9
    */
    
    // Leer la matriz desde el pipe (stdin)

    /*BMPImage* image;
    read(STDIN_FILENO, image, sizeof(image));
    
    //FOR PARA CORTAR IMAGE EN COLUMNAS A LEER
    //crear imagen recortada para filtros

    //FORS PARA LA CANTIDAD DE FILTROS
    BMPImage* new_image_saturada = saturate_bmp(image, factor_saturacion);
    // Enviar el resultado al pipe (stdout)
    write(STDOUT_FILENO, new_image_saturada, sizeof(new_image_saturada));


    write(STDOUT_FILENO, new_image_binarizada, sizeof(new_image_saturada));
    */
    return 0;
}
