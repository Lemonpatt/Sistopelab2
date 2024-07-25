#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "Bmp.h"

void create_worker(int pipe_fdw[2], int pipe_fdp[2], int id_worker);

BMPImage* create_image_worker(int id_lectura);

//void aplicar_filtros(BMPImage* image, int id_worker, int cantidad_filtros, double factor_saturacion, double umbral_binarizacion, BMPImage* resultados_filtros[]);

//void enviar_resultados(int id_envio, BMPImage* resultados_filtros[], int cantidad_filtros);