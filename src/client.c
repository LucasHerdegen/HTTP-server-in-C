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

    while (true)
    {
        // TODO: ENVIAR PETICION Y ESPERAR RESPUESTA
    }

    liberar_conexion(socket);

    return 0;
}