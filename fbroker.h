#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void create_worker(int cantidad_filtros, float factor_saturacion, float umbral_binarizacion, int pipe_fd[2]);