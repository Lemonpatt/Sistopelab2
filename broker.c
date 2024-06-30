#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "fbroker.h"
/*
para unir la imagen crear una matriz de tamaño:
image width x image height e irla rellenando con las partres de la imagen de los workers
*/


int main(int argc, char *argv[]) {



    //Crear en el main un execl de broker que reciba la cantidad de workers a crear
    int cantidad_workers = atoi(argv[1]);
    int pipe_fd[cantidad_workers][2];

    int cantidad_imagenes, cantidad_filtros, len;
    double factor_saturacion;
    double umbral_binarizacion;
    char * nombres_imagenes[100];
    BMPImage* image;
    // Leer parámetros desde stdin seguramente pasar a funcion a parte despues
    read(STDIN_FILENO, &cantidad_imagenes, sizeof(cantidad_filtros));
    read(STDIN_FILENO, &cantidad_filtros, sizeof(cantidad_filtros));
    read(STDIN_FILENO, &factor_saturacion, sizeof(factor_saturacion));
    read(STDIN_FILENO, &umbral_binarizacion, sizeof(umbral_binarizacion));
    printf("cantidad de filtros es %f", factor_saturacion);
    for (int i = 0; cantidad_imagenes > i; i++){
        image = (BMPImage*)malloc(sizeof(BMPImage));
        read(STDIN_FILENO, image, sizeof(BMPImage));
        printf("la resolucion de la imagen es %d por %d\n", image->height, image->width);
    }
    // Crear pipes y procesos workers
    /*for (int i = 0; i < cantidad_workers; i++) {
        if (pipe(pipe_fd[i]) == -1) {
            perror("pipe");
            exit(1);
        }
        create_worker(pipe_fd[i]);

        close(pipe_fd[i][0]); // Cerrar el extremo de lectura del pipe

        // Escribir los parámetros al pipe
        write(pipe_fd[i][1], &cantidad_filtros, sizeof(cantidad_filtros));
        write(pipe_fd[i][1], &factor_saturacion, sizeof(factor_saturacion));
        write(pipe_fd[i][1], &umbral_binarizacion, sizeof(umbral_binarizacion));
        close(pipe_fd[i][1]); // Cerrar el extremo de escritura del pipe
    }


    // Esperar a que todos los workers terminen y recoger resultados
    for (int i = 0; i < cantidad_workers; i++) {
        wait(NULL);
    }

    BMPImage** results[cantidad_workers];
    for (int i = 0; i < cantidad_workers; i++) {
        read(pipe_fd[i][0], results[i], sizeof(results[i]));
        close(pipe_fd[i][0]);
    }

    // Reconstruir imagen, se debe recibir basado en la cantidad de filtros cuantas veces se reconstruye la imagen


    return 0; */
}