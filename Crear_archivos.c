#include "Crear_archivos.h"

void make_folder(char *folder_name){
    int i = 0;
    char new_folder[200]; //Buffer de nuestro sprintf y variable que guarda el nombre
    while(1){
        if (i == 0){
            strcpy(new_folder, folder_name);
        } else {
            sprintf(new_folder, "%s_%d", folder_name, i);
        }

        if (mkdir(new_folder, 0777) == 0){
            printf("Se creo la carpeta %s\n", new_folder);
            //Se dio como puntero por lo que podemos reemplazarla directamente
            strcpy(folder_name, new_folder);
            return;
        }
        else {
            if (errno == EEXIST){
                i++;
            }
            else {
                perror("Error creando carpeta");
                exit(EXIT_FAILURE);
            }
        } 
    }
}

void make_csv(char *csv_name, float umbral){
    int i = 0;
    char new_csv[200]; //Buffer de nuestro sprintf y variable que guarda el nombre
    while(1){
        if (i == 0){
            sprintf(new_csv, "%s.csv", csv_name);
        } else {
            sprintf(new_csv, "%s_%d.csv", csv_name, i);
        }

        FILE *file = fopen(new_csv, "r");
        //Revisa que el archivo no exista
        if (file == NULL){
            file = fopen(new_csv, "w");
            if (file == NULL) {
                perror("Error creando archivo CSV");
                exit(EXIT_FAILURE);
            }
            fprintf(file, "Nombre Imagen - Filtro;Umbral clasificacion: %f\n", umbral);
            fclose(file);
            //Actualizamos el nombre del archivo al final
            strcpy(csv_name, new_csv);
            return;
        }
        //Solo llega aqui si encontro que el csv ya existia
        i++;
    }
}

