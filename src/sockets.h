#ifndef SOCKETS_H_
#define SOCKETS_H_

#define LOG_ERROR(msg) perror(msg)
#define LOG_INFO(msg) fprintf(stderr, "[INFO]: %s\n", msg)
#define LOG_WARNING(msg) fprintf(stderr, "\033[33m[WARNING]: %s\033[0m\n", msg)

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

typedef enum
{
	ERROR,
	MENSAJE,
	PAQUETE
} op_code;

typedef struct
{
	int socket;
	char *ip;
	char *puerto;
} c_modulo;

typedef struct
{
	int size;
	void *stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer *buffer;
} t_paquete;

void *recibir_buffer(int *, int);

int iniciar_servidor(char *puerto);

int esperar_cliente(int);

op_code recibir_operacion(int);

int crear_conexion(char *ip, char *puerto);

t_paquete *crear_paquete(void);

void agregar_a_paquete(t_paquete *paquete, void *valor, int tamanio);

void *serializar_paquete(t_paquete *paquete, int bytes);

void enviar_paquete(t_paquete *paquete, int socket_cliente);

void liberar_conexion(int socket_cliente);

void eliminar_paquete(t_paquete *paquete);

void crear_buffer(t_paquete *paquete);

void saludar();

#endif