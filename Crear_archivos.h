#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> //Hacemos uso de la funcion mkdir
#include <sys/types.h>
#include <errno.h>


extern int errno;

//Entradas: Char* (nombre de la carpeta)
//Salidas: void
//Descripción: Función que crea una carpeta con el nombre que se le pase
void make_folder(char* folder_name);

//Entradas: Char* (nombre del archivo), float (umbral)
//Salidas: void
//Descripción: Función que crea un archivo csv con el nombre que se le pase y un umbral
void make_csv(char *csv_name, float umbral);