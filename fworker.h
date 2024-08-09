#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "Bmp.h"


//Entradas: pipe_fdw, pipe_fdp, id_worker
//Salidas: void
//Descripción: Función que crea un worker
void create_worker(int pipe_fdw[2], int pipe_fdp[2], int id_worker);


//Entradas: id_lectura
//Salidas: BMPImage* (Imagen formato BMP)
//Descripción: Función que crea una imagen BMP a partir de un id de lectura
BMPImage* create_image_worker(int id_lectura);

//void aplicar_filtros(BMPImage* image, int id_worker, int cantidad_filtros, double factor_saturacion, double umbral_binarizacion, BMPImage* resultados_filtros[]);

//void enviar_resultados(int id_envio, BMPImage* resultados_filtros[], int cantidad_filtros);