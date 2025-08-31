#include "sockets.h"
#include "server.h"

const HttpRequest HttpError = {"", "", "", NULL, -1};

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
            LOG_WARNING("No se ha podido conectar el cliente...\n");
            close(*client_socket);
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
    
    bytes = recv(socket, buffer, sizeof(buffer) - 1, 0);

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

    buffer[bytes] = '\0';

    char* resp;

    LOG_INFO("Peticion recibida: ");
    fprintf(stderr, "%s\n", buffer);

    HttpRequest req = parse_request(buffer);

    if (req.header_count == -1)
    {
        LOG_WARNING("Protocolo HTTP no respetado, cerrando conexion. Socket: ");
        fprintf(stderr, "%d", socket);
        resp = "HTTP/1.1 400 Bad Request \r\n";
        send(socket, resp, strlen(resp), MSG_NOSIGNAL);
        close(socket);

        return NULL;
    }
/*
    LOG_INFO("Metodo: ");
    fprintf(stderr, "%s\n", req.method);
    LOG_INFO("Path: ");
    fprintf(stderr, "%s\n", req.path);
    LOG_INFO("Version: ");
    fprintf(stderr, "%s\n", req.version);
*/
    sleep(3);

    parse_headers(buffer, &req);
/*
    for (int i = 0; i < req.header_count; i++)
        fprintf(stderr, "Header[%d]: %s\n", i, req.headers[i]);

    sleep(3);
*/
    HttpResponse response = procesar_request(req);

    resp = generar_response(response);

    // char *resp = 
    // "HTTP/1.1 200 OK\r\n"
    // "Content-Type: text/plain\r\n"
    // "Content-Length: 12\r\n"
    // "Connection: close\r\n"
    // "\r\n"
    // "Hello World\n";

    LOG_INFO("Respondiendo con: \n");
    LOG_RESPONSE(resp);
    send(socket, resp, strlen(resp), MSG_NOSIGNAL);
    
    close(socket);
    free_request(&req);
    free_response(&response);

    return NULL;
}

HttpRequest parse_request(const char* request)
{
    HttpRequest req;
    memset(&req, 0, sizeof(HttpRequest));

    char first_line[256];
    sscanf(request, "%[^\r\n]", first_line);
    
    int variables_asignadas = sscanf(first_line, "%s %s %s", req.method, req.path, req.version);

    if (variables_asignadas != 3)
    return HttpError;

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

void free_request(HttpRequest* req)
{
    for (int i = 0; i < req->header_count; i++)
    {
        free(req->headers[i]);
    }
    free(req->headers);
}

HttpResponse procesar_request(HttpRequest request)
{
    HttpResponse response;
    memset(&response, 0, sizeof(HttpResponse));
    char* path = request.path;

    if (strcmp(path, "/") == 0)
    {
        strcpy(response.version, request.version);
        response.status_code = 200;
        strcpy(response.status_text, "OK");

        response.body = strdup("\nHello, World!\n\n");

        char content_lenght[50];
        snprintf(content_lenght, sizeof(content_lenght), "Content-Length: %d", (int)strlen(response.body));
        add_header(&response, content_lenght);
        char content_type[50];
        snprintf(content_type, sizeof(content_type), "Content-Type: %s", "text/plain");
        add_header(&response, content_type);
        add_header(&response, "Connection: close");
    }
    else if (strcmp(path, "/time") == 0)
    {
        strcpy(response.version, request.version);
        response.status_code = 200;
        strcpy(response.status_text, "OK");

        time_t tiempo_actual_raw;
        time(&tiempo_actual_raw);
        struct tm *tiempo_local;
        tiempo_local = localtime(&tiempo_actual_raw);
        response.body = malloc(64);
        strftime(response.body, 64, "\n%Y-%m-%d %H:%M:%S\n\n", tiempo_local);

        char content_lenght[50];
        snprintf(content_lenght, sizeof(content_lenght), "Content-Length: %d", (int)strlen(response.body));
        add_header(&response, content_lenght);
        char content_type[50];
        snprintf(content_type, sizeof(content_type), "Content-Type: %s", "text/plain");
        add_header(&response, content_type);
        add_header(&response, "Connection: close");
    }
    else
    {
        strcpy(response.version, request.version);
        response.status_code = 404;
        strcpy(response.status_text, "Not Found");

        add_header(&response, "Content-Length: 0");
        add_header(&response, "Connection: close");
    }

    return response;
}

char* generar_response(HttpResponse response)
{
    size_t total_len = 0;

    total_len += strlen(response.version) + 1; // espacio
    total_len += 3; // status code (ej: 200)
    total_len += 1; // espacio
    total_len += strlen(response.status_text);
    total_len += 2; // \r\n

    // headers
    for (int i = 0; i < response.header_count; i++) 
        total_len += strlen(response.headers[i]) + 2; // header + \r\n

    total_len += 2; // \r\n final

    // body
    if (response.body)
        total_len += strlen(response.body);

    char* resp = malloc(total_len + 1);
    if (!resp) return NULL;

    int offset = 0;

    // status line
    offset += sprintf(resp + offset, "%s %d %s\r\n",response.version, response.status_code,response.status_text);

    // headers
    for (int i = 0; i < response.header_count; i++) 
        offset += sprintf(resp + offset, "%s\r\n", response.headers[i]);

    // línea vacía
    offset += sprintf(resp + offset, "\r\n");

    // body
    if (response.body)
        offset += sprintf(resp + offset, "%s", response.body);

    // null terminator
    resp[offset] = '\0';

    return resp;
}


void add_header(HttpResponse* response, char* header)
{
    response->headers = realloc(response->headers, sizeof(char*) * (response->header_count + 1));
    response->headers[response->header_count++] = strdup(header);
}

void free_response(HttpResponse* response)
{
    for (int i = 0; i < response->header_count; i++)
    {
        free(response->headers[i]);
    }

    free(response->headers);

    if (response->body)
        free(response->body);
}