#include "sockets.h"
#include "server.h"

int main(int argc, char* argv[])
{
    char* server_port = puerto_param(argc, argv);

    int server_socket = iniciar_servidor("8008");

    saludar();

    while (true)
    {
        int client_socket = esperar_cliente(server_socket);

        if (client_socket == -1)
        {
            LOG_WARNING("No se ha podido conectar el cliente...");
            continue;
        }

        pthread_t handler_thread;
        pthread_create(&handler_thread, NULL, handler_cliente, (void*)client_socket);
    }

    return 0;
}

void* handler_cliente(void* client_socket)
{
    sleep(20);
}

char* puerto_param(int argc, char* argv[])
{
    if (argc < 2)
    {
        LOG_INFO("No se ha pasado puerto como parámetro. Utilizando el puerto 8000 por defecto.");
        return "8000";
    }

    return argv[1];
}