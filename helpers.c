
#include "helpers.h"
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void compute_message(char *message, const char *line)
{
    strcat(message, line);
    strcat(message, "\r\n");
}

int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag)
{
    struct sockaddr_in serv_addr;
    int sockfd = socket(ip_type, socket_type, flag);
    if (sockfd < 0)
        error("ERROR opening socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(portno);
    inet_aton(host_ip, &serv_addr.sin_addr);

    /* connect the socket */
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    return sockfd;
}

void close_connection(int sockfd)
{
    close(sockfd);
}

void send_to_server(int sockfd, char *message)
{
    int bytes, sent = 0;
    int total = strlen(message);

    do
    {
        bytes = write(sockfd, message + sent, total - sent);
        if (bytes < 0) {
            error("ERROR writing message to socket");
        }

        if (bytes == 0) {
            break;
        }

        sent += bytes;
    } while (sent < total);
}

char *receive_from_server(int sockfd)
{
    char response[BUFLEN];
    buffer buffer = buffer_init();
    int header_end = 0;
    int content_length = 0;

    do {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0){
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
        
        header_end = buffer_find(&buffer, HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

        if (header_end >= 0) {
            header_end += HEADER_TERMINATOR_SIZE;
            
            int content_length_start = buffer_find_insensitive(&buffer, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);
            
            if (content_length_start < 0) {
                continue;           
            }

            content_length_start += CONTENT_LENGTH_SIZE;
            content_length = strtol(buffer.data + content_length_start, NULL, 10);
            break;
        }
    } while (1);
    size_t total = content_length + (size_t) header_end;
    
    while (buffer.size < total) {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0) {
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
    }
    buffer_add(&buffer, "", 1);
    return buffer.data;
}
char* sendGetMessage(const char *url, const char *cookie, const char *auth) {
    int socket;
    char *servResponse, *message;

    socket = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    if (socket == -1) {
        exit(-1);
    }
    message = compute_get_request(HOST, url, "", cookie, auth);

    send_to_server(socket, message);
    servResponse = receive_from_server(socket);

    close_connection(socket);
    free(message);
    return servResponse;
}

char* sendPostMessage(const char *jsonString, const char *url, const char *auth) {
    int socket;
    char *message, *servResponse;
    socket = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    if (socket == -1) {
        exit(-1);
    }
    message = compute_post_request(HOST, url, "application/json", jsonString, "", auth);

    send_to_server(socket, message);
    servResponse = receive_from_server(socket);

    close_connection(socket);
    free(message);
    return servResponse;
}

char* sendDeleteMessage(const char *url, const char *auth) {
    int socket;
    char *message, *servResponse;
    socket = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    if (socket == -1) {
        exit(-1);
    }
    message = compute_delete_request(HOST, url, "application/json", auth);

    send_to_server(socket, message);
    servResponse = receive_from_server(socket);

    close_connection(socket);
    free(message);
    return servResponse;
}

int isNumber(const char *str) {
    // Check if the first character is a minus sign for negative numbers
    if (*str == '-') {
        ++str; // Skip the minus sign
    }

    // Check for an empty string or just "-"
    if (*str == '\0') {
        return 0;
    }

    // Check each character to see if it is a digit
    while (*str) {
        if (!isdigit((unsigned char)*str)) {
            return 0; // Return false if non-digit is found
        }
        str++;
    }

    return 1; // All characters were digits
}
char* extract_cookie(const char *response) {
    regex_t regex;
    regmatch_t matches[2]; // We need only one match, but regex functions expect an array
    char cookieContent[1024];
    // Compile regex. Check for errors.
    if (regcomp(&regex, "Set-Cookie: ([^\n\r]*)", REG_EXTENDED)) {
        fprintf(stderr, "Could not compile regex\n");
        return NULL;
    }
    if (regexec(&regex, response, 2, matches, 0) == 0) {
        int length = matches[1].rm_eo - matches[1].rm_so;
        strncpy(cookieContent, response + matches[1].rm_so, length);
        cookieContent[length] = '\0'; // Null-terminate the string
        // Free compiled regex
        regfree(&regex);
        return strdup(cookieContent);
    } else {
        // Free compiled regex
        regfree(&regex);
        return NULL;
    }
}