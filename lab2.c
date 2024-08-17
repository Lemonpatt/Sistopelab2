#include "fbroker.h"
#include "dirent.h"

int main(int argc, char *argv[]) {

    // Se inicializan los parametros a usar para getopt
    char *nombre_prefijo = NULL;
    char *nombre_imagenes[100] = {NULL};
    int cantidad_filtros = 0;
    double factor_saturacion = -1.0;
    double umbral_binarizacion = -1.0;
    double umbral_clasificacion = -1.0;
    char *nombre_carpeta = NULL;
    char *nombre_archivo_csv = NULL;
    int cantidad_workers = 1;
    int opt;
    int cantidad_imagenes = 0;
    char *endptr;


    // Procesar las opciones de línea de comandos
    while ((opt = getopt(argc, argv, "N:f:p:u:v:C:R:W:")) != -1) {
        switch (opt) {

            case 'N':
                nombre_prefijo = optarg;
                break;

            case 'f':
                cantidad_filtros = atoi(optarg);
                // Se revisa que el numero entregado sea valido
                if (cantidad_filtros < 1 || cantidad_filtros > 3) {
                    fprintf(stderr, "La cantidad de filtros debe ser de 1 a 3.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'p':
                factor_saturacion = atof(optarg);

                // Se revisa que el número entregado sea válido (Aqui se eligió que sea mayor que 0)
                if (factor_saturacion <= 0.0) {
                    fprintf(stderr, "Factor de saturacion no valido.\n");
                    fprintf(stderr, "El factor de saturacion debe ser un número mayor que 0.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'u':
                umbral_binarizacion = atof(optarg);
                //Umbral de binarización debe estar entre 0 y 1
                if (umbral_binarizacion < 0.0 || umbral_binarizacion > 1.0) {
                    fprintf(stderr, "El umbral de binarización debe estar entre 0 y 1.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'v':
                umbral_clasificacion = atof(optarg);
                //Umbral de clasificación debe estar entre 0 y 1
                if (umbral_clasificacion < 0.0 || umbral_clasificacion > 1.0) {
                    fprintf(stderr, "El umbral de clasificación debe estar entre 0 y 1.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'C':
                nombre_carpeta = optarg;
                break;

            case 'R':
                nombre_archivo_csv = optarg;
                break;

            case 'W':
                cantidad_workers = atoi(optarg);
                if (cantidad_workers < 1) {
                    fprintf(stderr, "La cantidad de workers debe ser 1 o más.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            default:
                fprintf(stderr, "Uso: %s -N <nombre_prefijo> -f <cantidad_filtros> -p <factor_saturacion> -u <umbral_binarizacion> -v <umbral_clasificacion> -C <nombre_carpeta> -R <nombre_archivo_csv>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    //Nos aseguramos que reciba algun parametro de prefijo y solamente uno de los 3 prefijos permitidos
    if (nombre_prefijo == NULL){
        fprintf(stderr, "Debe incluir el prefijo de las imagenes a procesar (-N).\n");
        exit(EXIT_FAILURE);   
    
    }

    if (cantidad_workers < 1){
        fprintf(stderr, "Se debe dar una cantidad valida de workers.\n");
        exit(EXIT_FAILURE);   
    
    }

    if (strcmp(nombre_prefijo, "imagen") && strcmp(nombre_prefijo, "img") && strcmp(nombre_prefijo, "photo")){
        fprintf(stderr, "El parametro para el prefijo debe ser imagen, img o photo.\n");
        exit(EXIT_FAILURE);   
    
    }


     // Se abre el directorio actual
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("Error al abrir el directorio actual");
        exit(EXIT_FAILURE);
    }


    struct dirent *entry;
    int sufijo = 1;
    while ((entry = readdir(dir)) != NULL) {
        // Verificar si el nombre comienza con el prefijo deseado y es un archivo .bmp
        if (strncmp(entry->d_name, nombre_prefijo, strlen(nombre_prefijo)) == 0 && (strcmp(entry->d_name + strlen(nombre_prefijo) + 2, ".bmp") == 0)) {
        
            // Es una imagen con el nombre adecuado, guardar el nombre
            const char *num_str = entry->d_name + strlen(nombre_prefijo);  // Apuntar a después del prefijo
            if (sscanf(num_str, "_%d", &sufijo) == 1){

                // Copiar el nombre de la imagen sin la extensión (.bmp)
                char *nombre_sin_extension = (char *)malloc(strlen(entry->d_name) - 4);  // Reservar memoria para el nuevo nombre
                strncpy(nombre_sin_extension, entry->d_name, strlen(entry->d_name) - 4);
                printf("nombre guardado: %s\n ", nombre_sin_extension);
                //Lo guardamos en la lista de imagenes y aumentamos el contador
                nombre_imagenes[cantidad_imagenes] = nombre_sin_extension;
                cantidad_imagenes++;

            }
            // Verificar si se alcanzó el máximo de nombres a guardar
            if (cantidad_imagenes == 100) {
                fprintf(stderr, "Advertencia: se ha alcanzado el límite máximo de nombres de imagenes a procesar.\n");
                    break;
                }
            //Aumentamos el contador de sufijo
            sufijo++;
        }
        
    }


    // Verificar si se proporcionaron todos los parámetros obligatorios
    if (nombre_imagenes[0] == NULL) {
        fprintf(stderr, "No se puede realizar la ejecución sin una imagen.\n");
        exit(EXIT_FAILURE);
    }

    if (nombre_carpeta == NULL || nombre_archivo_csv == NULL) {
        fprintf(stderr, "Es obligatorio especificar el nombre de la carpeta y csv con resultados.\n");
        exit(EXIT_FAILURE);
    }

    // Cambiar parametros no dados a sus valores por defecto
    if (cantidad_filtros == 0) {
        cantidad_filtros = 3;
    }
    if (factor_saturacion == -1.0) {
        factor_saturacion = 1.3;
    }

    if (umbral_binarizacion == -1.0) {
        umbral_binarizacion = 0.5;
    }
    if (umbral_clasificacion == -1.0) {
        umbral_clasificacion = 0.5;
    }


    // Mostrar los parámetros obtenidos
    //printf("Nombre del prefijo de las imágenes: %s\n", nombre_prefijo);
    printf("Cantidad de filtros a aplicar: %d\n", cantidad_filtros);
    printf("Factor de saturación del filtro: %f\n", factor_saturacion);
    printf("Umbral para binarizar la imagen: %f\n", umbral_binarizacion);
    printf("Umbral para clasificación: %f\n", umbral_clasificacion);
    printf("Nombre de la carpeta resultante con las imágenes procesadas: %s\n", nombre_carpeta);
    printf("Nombre del archivo CSV con las clasificaciones resultantes: %s\n", nombre_archivo_csv);



    // Creacion del broker ___________________________________________

    //Pipes comunicacion broker y padre
    int pipe_fdb[2];
    int pipe_fdp[2];
    if (pipe(pipe_fdb) == -1 ||pipe(pipe_fdp) == -1 ) {
        perror("pipe");
        exit(1);
    }

    create_broker(pipe_fdb, pipe_fdp, cantidad_workers);


    //Escribimos los parametros a el broker en el pipe______________________________________________________
    write(pipe_fdb[1], &cantidad_imagenes, sizeof(int));
    write(pipe_fdb[1], &cantidad_filtros, sizeof(int));
    write(pipe_fdb[1], &factor_saturacion, sizeof(double));
    write(pipe_fdb[1], &umbral_binarizacion, sizeof(double));

    BMPImage* image_S, *image_G, *image_B;

    //Creamos la carpeta con los resultados y el archivo csv
    make_folder(nombre_carpeta);
    make_csv(nombre_archivo_csv, umbral_clasificacion);

    FILE *csv = NULL;
    int resultadoNB;

    for (int i = 0; cantidad_imagenes > i; i++){

        char nombre_imagen[400];
        sprintf(nombre_imagen, "%s.bmp", nombre_imagenes[i]);
        
        printf("Imagen siendo procesada: %s\n", nombre_imagen);

        //Leemos la imagen
        BMPImage* image = read_bmp(nombre_imagen);
        if (!image) {
            return 1;
        }
        // Enviamos los datos de la imagen al broker
        write(pipe_fdb[1], &image->width, sizeof(int));
        write(pipe_fdb[1], &image->height, sizeof(int));
        
        for (int y = 0; y < image->height; y++) {
            for (int x = 0; x < image->width; x++) {
                RGBPixel pixel = image->data[y * image->width + x];
                write(pipe_fdb[1], &pixel.r, sizeof(unsigned char));
                write(pipe_fdb[1], &pixel.g, sizeof(unsigned char));
                write(pipe_fdb[1], &pixel.b, sizeof(unsigned char));
                //printf("IMAGEN: %s. MAIN pixel %dx%d, r: %d g: %d, b: %d\n", nombre_imagen, x, y, pixel.r, pixel.g, pixel.b);
            }
        }

        image_S = (BMPImage*)malloc(sizeof(BMPImage));
        image_S->height = image->height;
        image_S->width = image->width;
        image_S->data = (RGBPixel*)malloc(sizeof(RGBPixel) * image->width * image->height);

        if (cantidad_filtros > 1){
            image_G = (BMPImage*)malloc(sizeof(BMPImage));
            image_G->height = image->height;
            image_G->width = image->width;
            image_G->data = (RGBPixel*)malloc(sizeof(RGBPixel) * image->width * image->height);

            if(cantidad_filtros == 3){
                image_B = (BMPImage*)malloc(sizeof(BMPImage));
                image_B->height = image->height;
                image_B->width = image->width;
                image_B->data = (RGBPixel*)malloc(sizeof(RGBPixel) * image->width * image->height);
            }
        }
        
        for (int a = 0; a < image->height; a++){
            for (int b = 0; b < image->width; b++) {
                RGBPixel pixel;
                read(pipe_fdp[0], &pixel.r, sizeof(unsigned char));
                read(pipe_fdp[0], &pixel.g, sizeof(unsigned char));
                read(pipe_fdp[0], &pixel.b, sizeof(unsigned char));
                image_S->data[a * image->width + b] = pixel;
            }
        }

        //Otorgamos el filepath al nombre para escribirlo en la carpeta dada
        sprintf(nombre_imagen, "%s/%s_Saturated.bmp", nombre_carpeta, nombre_imagenes[i]);

        write_bmp(nombre_imagen, image_S);

        //Se abre en modo de adicion el archivo csv
        csv = fopen(nombre_archivo_csv, "a");

        //Revisamos si se hacen los siguientes filtros
        if (cantidad_filtros > 1){
            //Otorgamos el filepath al nombre para escribirlo en la carpeta dada
            sprintf(nombre_imagen, "%s/%s_Gris.bmp", nombre_carpeta, nombre_imagenes[i]);

            for (int a = 0; a < image->height; a++){
                for (int b = 0; b < image->width; b++) {
                    RGBPixel pixel;
                    read(pipe_fdp[0], &pixel.r, sizeof(unsigned char));
                    read(pipe_fdp[0], &pixel.g, sizeof(unsigned char));
                    read(pipe_fdp[0], &pixel.b, sizeof(unsigned char));
                    image_G->data[a * image->width + b] = pixel;
                }
            }

            write_bmp(nombre_imagen, image_G);

            if (cantidad_filtros == 3){

                //Otorgamos el filepath al nombre para escribirlo en la carpeta dada
                sprintf(nombre_imagen, "%s/%s_Binario.bmp", nombre_carpeta, nombre_imagenes[i]);

                for (int a = 0; a < image->height; a++){
                    for (int b = 0; b < image->width; b++) {
                        RGBPixel pixel;
                        read(pipe_fdp[0], &pixel.r, sizeof(unsigned char));
                        read(pipe_fdp[0], &pixel.g, sizeof(unsigned char));
                        read(pipe_fdp[0], &pixel.b, sizeof(unsigned char));
                        image_B->data[a * image->width + b] = pixel;
                    }
                }

                write_bmp(nombre_imagen, image_B);

                //Si se llegó hasta aca, quiere decir que se debe aplicar la funcion nearly_black aqui
                resultadoNB = nearly_black(image_B, umbral_clasificacion);
                fprintf(csv, "%s-Binarizada; %d\n", nombre_imagenes[i], resultadoNB);
                //Cerramos csv para que no se realice nearly_black con las otras imagenes filtradas
                csv = NULL;
            }
            //Se ve nearly black solo si no se ha hecho todavia
            if (csv != NULL){
                resultadoNB = nearly_black(image_G, umbral_clasificacion);
                fprintf(csv, "%s-Gris; %d\n", nombre_imagenes[i], resultadoNB);
                csv = NULL;
            }

        }
        //Se ve nearly black solo si no se ha hecho todavia
        if (csv != NULL){
            resultadoNB = nearly_black(image_S, umbral_clasificacion);
            fprintf(csv, "%s-Saturada; %d\n", nombre_imagenes[i], resultadoNB);
            csv = NULL;
        }
        //Liberamos las imagenes creadas y la original
        free_bmp(image);
        free_bmp(image_S);
        free_bmp(image_G);
        free_bmp(image_B);
    }
    close(pipe_fdb[1]);
    close(pipe_fdp[0]);
    wait(NULL);
    
    return 0;
}