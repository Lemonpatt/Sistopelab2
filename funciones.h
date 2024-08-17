#include "Bmp.h"


/*
Entradas: un puntero a BMPImage y un flotante que representa el factor de saturacion
Salidas: un puntero a BMPImage
Descripcion: Funcion que satura una imagen BMP
*/
BMPImage* saturate_bmp(BMPImage* image, float factor);

/*
Entradas: un puntero a BMPImage
Salidas: un puntero a BMPImage
Descripcion: Funcion que convierte una imagen BMP a escala de grises
*/
BMPImage* greyScale_bmp(BMPImage* image);

/*
Entradas: un puntero a BMPImage y un flotante que representa el umbral de binarizacion
Salidas: un puntero a BMPImage
Descripcion: Funcion que Binariza una imagen BMP antes convertida en escala de grises
*/
BMPImage* Binarizar_bmp(BMPImage* image, float umbral);

/*
Entradas: un puntero a BMPImage y un flotante que representa el umbral de binarizacion
Salidas: un entero 1 si la imagen es "casi negra" y 0 = si la imagen no es "casi negra"
Descripcion: Funcion que te dice si la imagen es "casi negra" dependiendo del umbral indicado
*/
int nearly_black(BMPImage * image, float umbral);