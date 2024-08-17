#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include "funciones.h"

/*
Entradas: Dos pipes y un entero que corresponde al id del worker
Salidas: vacio
Descripcion: Funcion que crea un worker con el Id indicado
*/
void create_worker(int pipe_fdw[2], int pipe_fdp[2], int id_worker);

/*
Entradas: un entero que corresponde al id de lectura
Salidas: un puntero a BMPImage
Descripcion: Funcion que crea una imagen BMP a partir de los datos recibidos
*/
BMPImage* create_image_worker(int id_lectura);

/*
Entradas: una imagen BMP a la que se le aplican los filtros, la cantidad de filtros a aplicar,
el factor de saturacion, el umbral de binarizacion y un arreglo de BMPImage en donde se almacenan
los resultados
Salidas: Vacio
Descripcion: Funcion que aplica los filtros correspondientes a una imagen con el factor de saturacion
y el umbral de binarizacion indicados
*/
void aplicar_filtros(BMPImage* image, int cantidad_filtros, double factor_saturacion, double umbral_binarizacion, BMPImage* resultados_filtros[]);

/*
Entradas: Un id de envio(int), un arreglo de imagenes BMP y la cantidad de filtros(int)
Salidas: Vacio
Descripcion: Funcion que envia los resultados de los filtros a traves de un pipe
*/
void enviar_resultados(int id_envio, BMPImage* resultados_filtros[], int cantidad_filtros);