#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fworker.h"

void create_broker(int pipe_fdb[2], int pipe_fdp[2], int cantidad_workers);

void send_image(BMPImage* image, int id_envio);