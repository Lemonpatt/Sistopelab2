#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fworker.h"


/*
Entradas: Recibe dos pipes y un entero correspondiente a la cantidad de workers
Salidas: Vacio
Descripcion: Funcion que crea un broker con la cantidad de workers indicada
*/
void create_broker(int pipe_fdb[2], int pipe_fdp[2], int cantidad_workers);

/*
Entradas: Recibe un puntero a BMPImage y un entero correspondiente al id del envio
Salidas: Vacio
Descripcion: Reconstruye la imagen BMP a partir de los resultados de los workers
*/
void send_image(BMPImage* image, int id_envio);