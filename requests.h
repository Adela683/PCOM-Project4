#ifndef _REQUESTS_
#define _REQUESTS_
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char* compute_get_request(const char* host, const char* url, const char* query_params, const char* cookies, const char* auth);
char* compute_post_request(const char* host, const char* url, const char* content_type, const char* body_data, const char* cookies, const char* auth);
char* compute_delete_request(const char* host, const char* url, const char* content_type, const char* auth);
#endif
