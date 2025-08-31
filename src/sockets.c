#include "sockets.h"

int iniciar_servidor(char *puerto)
{
	int socket_servidor, err;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(NULL, puerto, &hints, &servinfo);

	socket_servidor = socket(servinfo->ai_family,
							 servinfo->ai_socktype,
							 servinfo->ai_protocol);

	err = setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

	if (err == -1)
	{
		LOG_ERROR("setsockopt");
		exit(EXIT_FAILURE);
	}

	err = bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

	if (err == -1)
	{
		LOG_ERROR("bind");
		exit(EXIT_FAILURE);
	}

	err = listen(socket_servidor, SOMAXCONN);

	if (err == -1)
	{
		LOG_ERROR("listen");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(servinfo);

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	int socket_cliente = accept(socket_servidor, NULL, NULL);

	return socket_cliente;
}

int crear_conexion(char *ip, char *puerto)
{
	struct addrinfo hints;

	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
	if (socket_cliente == -1)
	{
		freeaddrinfo(server_info);
		return -1;
	}

	if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
	{
		freeaddrinfo(server_info);
		return -1;
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

void saludar()
{
    LOG_INFO("Hola");
    printf("Mundo \n");
    LOG_WARNING("WARNING XXXX");
}

char* puerto_param(int argc, char* argv[])
{
    if (argc < 2)
    {
        LOG_INFO("No se ha pasado puerto como parámetro. Utilizando el puerto 8000 por defecto.\n");
        return "8000";
    }

    return argv[1];
}