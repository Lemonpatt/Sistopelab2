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
    int id_envio = atoi(argv[2]);
    int id_lectura = atoi(argv[3]);
    //Pipe de worker
    int pipe_fdw[cantidad_workers][2];
    //Pipe de broker
    int pipe_fdb[cantidad_workers][2];
    int i = 0;
    int cantidad_imagenes, cantidad_filtros, len;
    double factor_saturacion;
    double umbral_binarizacion;
    char * nombres_imagenes[100];
    RGBPixel pixel;
    close(id_envio);
    // Leer parámetros desde stdin seguramente pasar a funcion a parte despues
    read(id_lectura, &cantidad_imagenes, sizeof(int));
    read(id_lectura, &cantidad_filtros, sizeof(int));
    read(id_lectura, &factor_saturacion, sizeof(double));
    read(id_lectura, &umbral_binarizacion, sizeof(double));
    while(i<cantidad_imagenes){
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

            if (pipe(pipe_fdw[j]) == -1 || pipe(pipe_fdb[j]) == -1) {
                perror("pipe");
                exit(1);
            }
            int total_columns = image->width;
            int columns_per_worker = total_columns / cantidad_workers;
            int start_column = j * columns_per_worker;
            int end_column = (j == cantidad_workers-1) ? total_columns : start_column + columns_per_worker;

            create_worker(pipe_fdw[j], pipe_fdb[j], j+1);
            int a;

            // Escribir los parámetros al pipe
            write(pipe_fdw[j][1], &cantidad_workers, sizeof(int));
            write(pipe_fdw[j][1], &cantidad_filtros, sizeof(int));
            write(pipe_fdw[j][1], &factor_saturacion, sizeof(double));
            write(pipe_fdw[j][1], &umbral_binarizacion, sizeof(double));

            //Esto será la nueva anchura de la imagen que recibe el worker
            int num_columns = end_column - start_column;            
            write(pipe_fdw[j][1], &num_columns, sizeof(int));

            //La altura se mantiene igual
            write(pipe_fdw[j][1], &image->height, sizeof(int)); 
            
            //Enviamos la informacion de la seccion de la imagen del worker
            for (int y = 0; y < image->height; y++) {
                for (int x = start_column; x < end_column; x++) {
                    RGBPixel pixel = image->data[y * image->width + x];
                    write(pipe_fdw[j][1], &pixel.r, sizeof(unsigned char));
                    write(pipe_fdw[j][1], &pixel.g, sizeof(unsigned char));
                    write(pipe_fdw[j][1], &pixel.b, sizeof(unsigned char));
                    //printf("IMAGEN: %s. MAIN pixel %dx%d, r: %d g: %d, b: %d\n", nombre_imagen, x, y, pixel.r, pixel.g, pixel.b);
                }
            }
            close(pipe_fdw[j][1]);
            read(pipe_fdb[j][0], &a, sizeof(a));
            printf("recibido: %d\n", a);

            close(pipe_fdb[j][0]); // Cerrar el extremo de lectura del pipe
        }
        write_bmp("testBROKER.bmp", image);

        free_bmp(image);
        // Esperar a que todos los workers terminen y recoger resultados

        int new_fd = dup(id_lectura);
        close(id_lectura);
        id_lectura = new_fd;
        i++;
    }
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