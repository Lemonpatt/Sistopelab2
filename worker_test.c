#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include"Filtros.h"

int main(int argc, char *argv[]) {
    
    int id_worker = atoi(argv[1]);
    int id_envio = atoi(argv[2]);
    int id_lectura = atoi(argv[3]);

    int cantidad_workers, cantidad_filtros;
    double factor_saturacion;
    double umbral_binarizacion;
    BMPImage* image;

    image = (BMPImage*)malloc(sizeof(BMPImage));

    char nombre_imagen[400];

    read(id_lectura, &cantidad_workers, sizeof(cantidad_workers));
    read(id_lectura, &cantidad_filtros, sizeof(cantidad_filtros));
    read(id_lectura, &factor_saturacion, sizeof(factor_saturacion));
    read(id_lectura, &umbral_binarizacion, sizeof(umbral_binarizacion));
    read(id_lectura, image, sizeof(BMPImage));
    
    int total_columns = image->width;
    int columns_per_worker = total_columns / cantidad_workers;
    int start_column = id_worker-1 * columns_per_worker;
    int end_column = (id_worker == cantidad_workers) ? total_columns - start_column : start_column + columns_per_worker;

    int num_columns = end_column - start_column;

    BMPImage resultados_filtros[cantidad_filtros];

    aplicar_filtros(image, id_worker, cantidad_filtros, factor_saturacion, umbral_binarizacion, resultados_filtros);


    printf("ID WORKER: %d \n Cantidad de workers son %d\n", id_worker, cantidad_workers);
    

    sprintf(nombre_imagen, "%s/a_Saturated.bmp", nombre_carpeta);
        //Aplicamos filtro de saturacion primero
        BMPImage* new_image = saturate_bmp(image, factor_saturacion);
        write_bmp(nombre_imagen, new_image);
    /* ej
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