#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Bmp.h"

void create_worker(int pipe_fdw[2], int id_worker);

RGBPixel* get_worker_image_section(BMPImage* image, RGBPixel* section_data, int id_worker, int cantidad_workers);