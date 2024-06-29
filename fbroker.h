#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Bmp.h"
#include "fworker.h"

void create_broker(int pipe_fd[2]);

void reconstruct_image(BMPImage* original_image, BMPImage** results, int num_workers);