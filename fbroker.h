#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fworker.h"


//Entradas: arreglo de enteros que contiene los file descriptors del pipe, y la cantidad de workers
//Salidas: void
//Descripción: Función que crea un broker, el cual se encarga de coordinar a los workers
void create_broker(int pipe_fd[2], int cantidad_workers);

//Entradas: BMPImage*(imagen formato BMP), BMPImage**(puntero a imagen formato BMP), y la cantidad de workers
//Salidas: void
//Descripción: Función para reconstruir la imagen BMP a partir de los resultados de los workers
void reconstruct_image(BMPImage* original_image, BMPImage** results, int num_workers);