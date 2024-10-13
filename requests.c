#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* compute_get_request(const char* host, const char* url, const char* query_params, const char* cookies, const char* auth) {
    char* message = malloc(1024); // Starting buffer size, adjust if necessary.
    if (message == NULL) {
        return NULL; // Memory allocation failed.
    }
    message[0] = '\0'; // Initialize the buffer as an empty string.

    if (query_params && strlen(query_params) > 0) {
        sprintf(message, "GET %s?%s HTTP/1.1\r\n", url, query_params);
    } else {
        sprintf(message, "GET %s HTTP/1.1\r\n", url);
    }

    sprintf(message + strlen(message), "Host: %s\r\n", host);

    if (auth && strlen(auth) > 0) {
        sprintf(message + strlen(message), "Authorization: Bearer %s\r\n", auth);
    }
    if (cookies && strlen(cookies) > 0) {
        sprintf(message + strlen(message), "Cookie: %s\r\n", cookies);
    }

    strcat(message, "\r\n");
    return message;
}

char* compute_post_request(const char* host, const char* url, const char* content_type, const char* body_data, const char* cookies, const char* auth) {
    int initial_size = strlen(body_data) + 1024; // Adjust buffer size for body data and headers.
    char* message = malloc(initial_size);
    if (message == NULL) {
        return NULL; // Memory allocation failed.
    }
    message[0] = '\0'; // Initialize the buffer as an empty string.

    sprintf(message, "POST %s HTTP/1.1\r\n", url);
    sprintf(message + strlen(message), "Host: %s\r\n", host);
    sprintf(message + strlen(message), "Content-Type: %s\r\n", content_type);
    sprintf(message + strlen(message), "Content-Length: %lu\r\n", strlen(body_data));

    if (auth && strlen(auth) > 0) {
        sprintf(message + strlen(message), "Authorization: Bearer %s\r\n", auth);
    }
    if (cookies && strlen(cookies) > 0) {
        sprintf(message + strlen(message), "Cookie: %s\r\n", cookies);
    }

    strcat(message, "\r\n");
    strcat(message, body_data);
    strcat(message, "\r\n");

    return message;
}

char* compute_delete_request(const char* host, const char* url, const char* content_type, const char* auth) {
    char* message = malloc(512); // Buffer size might need adjustment.
    if (message == NULL) {
        return NULL; // Memory allocation failed.
    }
    message[0] = '\0'; // Initialize the buffer as an empty string.

    sprintf(message, "DELETE %s HTTP/1.1\r\n", url);
    sprintf(message + strlen(message), "Host: %s\r\n", host);
    sprintf(message + strlen(message), "Content-Type: %s\r\n", content_type);

    if (auth && strlen(auth) > 0) {
        sprintf(message + strlen(message), "Authorization: Bearer %s\r\n", auth);
    }

    strcat(message, "\r\n");
    return message;
}
