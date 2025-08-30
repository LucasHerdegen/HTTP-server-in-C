#include "sockets.h"

int main(int argc, char* argv[])
{
    char* puerto = puerto_param(argc, argv);

    LOG_INFO("Conectando al servidor en puerto: ");
    fprintf(stderr, "%s\n", puerto);

    int socket = crear_conexion("127.0.0.1", puerto);



    return 0;
}