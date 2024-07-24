#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "Bmp.h"

void create_worker(int pipe_fdw[2], int id_worker);

BMPImage* get_worker_image_section(BMPImage* image, int id_worker, int cantidad_workers);