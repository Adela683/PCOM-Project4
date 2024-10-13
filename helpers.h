#ifndef _HELPERS_
#define _HELPERS_
#include "buffer.h"
#include "requests.h"
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <regex.h>
#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

#define BUFLEN 4096
#define LINELEN 1000
#define HOST "34.246.184.49"
#define PORT 8080
enum RequestType { GET, POST, DELETE };
// shows the current error
void error(const char *msg);

// adds a line to a string message
void compute_message(char *message, const char *line);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, char *message);

// receives and returns the message from a server
char *receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
int isNumber(const char *str);
char* sendPostMessage(const char *jsonString, const char *url, const char *auth);
char* sendGetMessage(const char *url, const char *cookie, const char *auth);
char* sendDeleteMessage(const char *url, const char *auth);
int isLoggedIn(char **cookie);
char* extract_cookie(const char *response);

#endif
