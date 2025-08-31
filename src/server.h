#include "sockets.h"

typedef struct
{
    char method[8]; // "GET", "POST"
    char path[256]; // "/" "/index.html"
    char version[16]; // "HTTP/1.1"
    char** headers;
    int header_count;
} HttpRequest;

typedef struct {
    char version[16];
    int status_code;
    char status_text[64];
    char** headers;
    int header_count;
    char* body;
} HttpResponse;

const HttpRequest HttpError;

void* handler_cliente(void* client_socket);

HttpRequest parse_request(const char* request);

void parse_headers(const char* request, HttpRequest* req);

void free_request(HttpRequest* req);

HttpResponse procesar_request(HttpRequest request);

char* generar_response(HttpResponse response);

void add_header(HttpResponse* response, char* header);

void free_response(HttpResponse* response);