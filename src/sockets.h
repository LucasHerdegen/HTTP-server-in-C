#ifndef SOCKETS_H_
#define SOCKETS_H_

#define LOG_ERROR(msg) perror(msg)
#define LOG_INFO(msg) fprintf(stderr, "[INFO]: %s", msg)
#define LOG_WARNING(msg) fprintf(stderr, "\033[33m[WARNING]: %s\033[0m", msg)
#define LOG_RESPONSE(msg) fprintf(stderr, "\033[32m%s\033[0m\n", msg)

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

int iniciar_servidor(char *puerto);

int esperar_cliente(int);

int crear_conexion(char *ip, char *puerto);

void liberar_conexion(int socket_cliente);

void saludar();

char* puerto_param(int argc, char* argv[]);

#endif