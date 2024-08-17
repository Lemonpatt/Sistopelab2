#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> //Hacemos uso de la funcion mkdir
#include <sys/types.h>
#include <errno.h>


extern int errno;

/*
Entradas: Recibe un puntero apuntando al nombre de la carpeta a crear
Salidas: Retorna vacio
Descripcion: Esta funcion crea una carpeta nombrada como el char* de la entrada, en caso de que
ya exista esa carpeta, crea una nueva con un numero añadido que aumenta hasta que no se repita.
*/
void make_folder(char* folder_name);

/*
Entradas: Recibe un puntero apuntando al nombre del archivo csv y un numero float (entre 0 y 1)
Salidas: Retorna vacio
Descripcion: Esta funcion crea un archivo csv que se llama como el nombre indicado, si ya existe
entonces se añade un numero hasta que no se repita y muestra el umbral que es el float que recibe
dentro del csv creado.
*/
void make_csv(char *csv_name, float umbral);