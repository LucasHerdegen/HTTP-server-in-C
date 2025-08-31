#include "sockets.h"

typedef struct
{
    char method[8]; // "GET", "POST"
    char path[256]; // "/" "/index.html"
    char version[16]; // "HTTP/1.1"
    char** headers;
    int header_count;
} HttpRequest;

void* handler_cliente(void* client_socket);

HttpRequest parse_request(const char* request);

void parse_headers(const char* request, HttpRequest* req);