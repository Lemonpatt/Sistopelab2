#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fworker.h"

void create_broker(int pipe_fd[2], int cantidad_workers);

void reconstruct_image(BMPImage* original_image, BMPImage** results, int num_workers);