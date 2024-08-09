#include "fbroker.h"


//Entradas: Recibe un puntero a una estructura BMPImage y un numero float( >= 0)
//Salidas: Retorna un puntero a una estructura BMPImage con sus datos de imagen saturados
//Descripcion: Funcion que satura los colores de la imagen multiplicando los colores por el float recibido
BMPImage* saturate_bmp(BMPImage* image, float factor);

//Entradas: Recibe un puntero a una estructura BMPImage
//Salidas: Retorna un puntero a una estructura BMPImage con sus datos de imagen ahora en tonos de grises
//Descripcion: Funcion que convierte una imagen a su version en escala de grises usando el espacio de color con valor luma ponderado de Rec.601
BMPImage* greyScale_bmp(BMPImage* image);

//Entradas: Recibe un puntero a una estructura BMPImage y un numero flotante (entre 0 y 1)
//Salidas: Retorna un puntero a una estructura BMPImage con su información de imagen binarizada
//Descripcion: Funcion que Binariza una imagen(la convierte en una imagen blanco y negro)
BMPImage* Binarizar_bmp(BMPImage* image, float umbral);

//Entradas:Recibe un puntero a una estructura BMPImage y un numero flotante (entre 0 y 1)
//Salidas: Retorna un numero entero (0 para false y 1 para true)
//Descripcion: Funcion que revisa si una imagen es "casi negra" basandose en el umbral (numero flotante) recibido, si es más alto o igual que el umbral entonces retorna 1
int nearly_black(BMPImage* image, float umbral);