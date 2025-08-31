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
    free(client_socket);

    char buffer[1024];
    int bytes;
    
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

    HttpRequest req = parse_request(buffer);

    LOG_INFO("Metodo: ");
    fprintf(stderr, "%s\n", req.method);
    LOG_INFO("Path: ");
    fprintf(stderr, "%s\n", req.path);
    LOG_INFO("Version: ");
    fprintf(stderr, "%s\n", req.version);

    sleep(3);

    parse_headers(buffer, &req);

    for (int i = 0; i < req.header_count; i++)
    {
        fprintf(stderr, "Header[%d]: %s\n", i, req.headers[i]);
    }

    sleep(3);

    char *resp = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 12\r\n"
    "Connection: close\r\n"
    "\r\n"
    "Hello World\n";

    LOG_INFO("Respondiendo con: \n");
    LOG_RESPONSE(resp);
    send(socket, resp, strlen(resp), 0);
    
    close(socket);
    return NULL;
}

HttpRequest parse_request(const char* request)
{
    HttpRequest req;
    memset(&req, 0, sizeof(HttpRequest));

    char first_line[256];
    sscanf(request, "%[^\r\n]", first_line);
    
    sscanf(first_line, "%s %s %s", req.method, req.path, req.version);

    return req;
}

void parse_headers(const char* request, HttpRequest* req) {
    req->headers = NULL;
    req->header_count = 0;

    const char* headers_start = strstr(request, "\r\n") + 2;
    const char* current = headers_start;

    while (current && *current != '\r' && *current != '\n')
    {
        const char* line_end = strstr(current, "\r\n");
        if (!line_end) break;

        size_t line_len = line_end - current;
        char* header = malloc(line_len + 1);
        strncpy(header, current, line_len);
        header[line_len] = '\0';

        req->headers = realloc(req->headers, sizeof(char*) * (req->header_count + 1));
        req->headers[req->header_count++] = header;

        current = line_end + 2;
    }
}
