#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fworker.h"


void create_worker(int pipe_fd[2]) {
    if (fork() == 0) {
        close(pipe_fd[1]); // Cerrar el extremo de escritura del pipe

        dup2(pipe_fd[0], STDIN_FILENO); // Redirigir stdin para leer desde el pipe
        close(pipe_fd[0]);

        execl("./broker", "broker", NULL); // Ejecutar el worker
        perror("execl");
        exit(1);
    }
}