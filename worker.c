#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include"Bmp.h"

int main(int argc, char *argv[]) {
    
    int columna_inicial, num_columnas;
    
    read(STDIN_FILENO, &columna_inicial, sizeof(columna_inicial));
    read(STDIN_FILENO, &num_columnas, sizeof(num_columnas));
    /* ej
    columna inicial 0
    n colum 10
    columna final = columna inicial + n colum - 1 = 9
    */
    
    // Leer la matriz desde el pipe (stdin)

    BMPImage* image;
    read(STDIN_FILENO, image, sizeof(image));
    
    //FOR PARA CORTAR IMAGE EN COLUMNAS A LEER
    //crear imagen recortada para filtros

    //FORS PARA LA CANTIDAD DE FILTROS
    BMPImage* new_image_saturada = saturate_bmp(image, factor_saturacion);
    // Enviar el resultado al pipe (stdout)
    write(STDOUT_FILENO, new_image_saturada, sizeof(new_image_saturada));


    write(STDOUT_FILENO, new_image_binarizada, sizeof(new_image_saturada));
    return 0;
}
