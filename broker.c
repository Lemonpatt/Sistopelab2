#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Filtros.h"
/*
para unir la imagen crear una matriz de tamaño:
image width x image height e irla rellenando con las partres de la imagen de los workers
*/


int main(int argc, char *argv[]) {



    //Crear en el main un execl de broker que reciba la cantidad de workers a crear
    int cantidad_workers = atoi(argv[1]);
    int id_envio = atoi(argv[2]);
    int id_lectura = atoi(argv[3]);
    int pipe_fdw[2];

    int cantidad_imagenes, cantidad_filtros, len;
    double factor_saturacion;
    double umbral_binarizacion;
    char * nombres_imagenes[100];
    RGBPixel pixel;
    close(id_envio);
    // Leer parámetros desde stdin seguramente pasar a funcion a parte despues
    read(id_lectura, &cantidad_imagenes, sizeof(cantidad_imagenes));
    read(id_lectura, &cantidad_filtros, sizeof(cantidad_filtros));
    read(id_lectura, &factor_saturacion, sizeof(factor_saturacion));
    read(id_lectura, &umbral_binarizacion, sizeof(umbral_binarizacion));

        BMPImage* image = (BMPImage*)malloc(sizeof(BMPImage));
        
        read(id_lectura, &image->width, sizeof(int));
        read(id_lectura, &image->height, sizeof(int));
        image->data = (RGBPixel*)malloc(sizeof(RGBPixel) * image->width * image->height);
        for (int y = 0; y < image->height; y++) {
            for (int x = 0; x < image->width; x++) {
                read(id_lectura, &pixel.r, sizeof(unsigned char));
                read(id_lectura, &pixel.g, sizeof(unsigned char));
                read(id_lectura, &pixel.b, sizeof(unsigned char));
                image->data[y * image->width + x] = pixel;
            }
        }

        // Crear pipes y procesos workers
        for (int j = 0; j < cantidad_workers; j++) {
            
            if (pipe(pipe_fdw) == -1) {
                perror("pipe");
                exit(1);
            }
            create_worker(pipe_fdw, j+1);

            close(pipe_fdw[0]); // Cerrar el extremo de lectura del pipe

            // Escribir los parámetros al pipe
            write(pipe_fdw[1], &cantidad_workers, sizeof(cantidad_workers));
            write(pipe_fdw[1], &cantidad_filtros, sizeof(cantidad_filtros));
            write(pipe_fdw[1], &factor_saturacion, sizeof(factor_saturacion));
            write(pipe_fdw[1], &umbral_binarizacion, sizeof(umbral_binarizacion));

            
            write(pipe_fdw[1], image, sizeof(image)); 
            
        }
        close(pipe_fdw[1]);
        write_bmp("testBROKER.bmp", image);
        BMPImage* new_image = saturate_bmp(image, factor_saturacion);
        write_bmp("testBROKER_Saturado.bmp", new_image);
        free_bmp(image);
        // Esperar a que todos los workers terminen y recoger resultados
        for (int i = 0; i < cantidad_workers; i++) {
            wait(NULL);
        }
    close(id_lectura);

/*


    BMPImage** results[cantidad_workers];
    for (int i = 0; i < cantidad_workers; i++) {
        read(pipe_fd[i][0], results[i], sizeof(results[i]));
        close(pipe_fd[i][0]);
    }

    // Reconstruir imagen, se debe recibir basado en la cantidad de filtros cuantas veces se reconstruye la imagen

*/
    return 0; 
}