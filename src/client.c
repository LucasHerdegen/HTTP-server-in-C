#include "sockets.h"
#include "client.h"

int main(int argc, char* argv[])
{
    char* puerto = puerto_param(argc, argv);

    LOG_INFO("Conectando al servidor en puerto: ");
    fprintf(stderr, "%s\n", puerto);

    int socket = crear_conexion("127.0.0.1", puerto);

    if (socket == -1)
    {
        LOG_ERROR("No se pudo conectar al servidor");
        exit(1);
    }

    LOG_INFO("Se ha conectado al servidor. Socket: ");
    fprintf(stderr, "%d\n", socket);

    char *req = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    send(socket, req, strlen(req), 0);

    char buffer[1024];
    int bytes = recv(socket, buffer, sizeof(buffer), 0);

    if (bytes <= 0)
    {
        LOG_ERROR("Se ha producido un error con el servidor al recibir la response. Abortando...\n");
        exit(1);
    }

    LOG_RESPONSE(buffer);

    liberar_conexion(socket);

    return 0;
}