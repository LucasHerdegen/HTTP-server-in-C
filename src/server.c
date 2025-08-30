#include "sockets.h"
#include "client.h"

int main(int argc, char** argv)
{
    int server_socket = iniciar_servidor("8008");

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