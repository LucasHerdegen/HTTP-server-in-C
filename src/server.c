#include "sockets.h"
#include "server.h"

int main(int argc, char* argv[])
{
    char* server_port = puerto_param(argc, argv);

    int server_socket = iniciar_servidor(server_port);

    while (true)
    {
        int* client_socket = malloc(sizeof(int));
        *client_socket = esperar_cliente(server_socket);

        if (*client_socket == -1)
        {
            LOG_WARNING("No se ha podido conectar el cliente...");
            continue;
        }

        LOG_INFO("Cliente conectado. Socket: ");
        fprintf(stderr, "%d\n", *client_socket);

        pthread_t handler_thread;
        pthread_create(&handler_thread, NULL, handler_cliente, (void*)client_socket);
        pthread_detach(handler_thread);
    }

    return 0;
}

void* handler_cliente(void* client_socket)
{
    int socket = *(int*)client_socket;

    char buffer[1024];
    int bytes;

    while (true)
    {
        bytes = recv(socket, buffer, sizeof(buffer), 0);

        if (bytes < 0)
        {
            LOG_WARNING("Ha ocurrido un error al recibir los datos. Socket: ");
            fprintf(stderr, "%d\n", socket);
            return NULL;
        }
        
        if (bytes == 0)
        {
            LOG_INFO("Cliente cerrado correctamente. Socket: ");
            fprintf(stderr, "%d\n", socket);
            return NULL;
        }

        LOG_INFO("Peticion recibida: ");
        fprintf(stderr, "%s\n", buffer);

        // TODO: MANEJAR PETICION
        
    }

}