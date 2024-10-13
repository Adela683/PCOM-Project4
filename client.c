#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

#include "helpers.h"
// Helper function to extract JSON body from HTTP response
char* extract_json_body(char *httpResponse) {
    // Find the first occurrence of a blank line, which separates headers and body
    char *body = strstr(httpResponse, "\r\n\r\n");
    if (body != NULL) {
        return body + 4; // Skip the "\r\n\r\n"
    }
    return NULL; // No body found
}

void handle_json_response_register(char* httpResponse) {
    char *jsonBody = extract_json_body(httpResponse);
    if (jsonBody == NULL) {
        printf("No JSON content found in the response.\n");
        return;
    }

    // Parse the JSON content of the response
    cJSON *json = cJSON_Parse(jsonBody);
    if (json == NULL) {
        printf("SUCCESS: Registration was successfully.\n");
    } else {
        cJSON *error = cJSON_GetObjectItemCaseSensitive(json, "error");
        if (cJSON_IsString(error) && (error->valuestring != NULL)) {
            printf("ERROR: %s\n", error->valuestring);
        }
        cJSON_Delete(json);
    }
}
int handle_json_response_login(char* httpResponse) {
    char *jsonBody = extract_json_body(httpResponse);
    if (jsonBody == NULL) {
        printf("No JSON content found in the response.\n");
        return 0;
    }

    // Parse the JSON content of the response
    cJSON *json = cJSON_Parse(jsonBody);
    if (json == NULL) {
        printf("SUCCESS: Login was successfully.\n");
        return 1;
    } else {
        cJSON *error = cJSON_GetObjectItemCaseSensitive(json, "error");
        if (cJSON_IsString(error) && (error->valuestring != NULL)) {
            printf("Error: %s\n", error->valuestring);
        }
        cJSON_Delete(json);
        return 0;
    }
}
void handle_book_response(char* jsonResponse) {
    cJSON *json = cJSON_Parse(jsonResponse);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        fprintf(stderr, "Failed to parse JSON.\n");
    } else {
        // Check if there is an error message in the response
        cJSON *error = cJSON_GetObjectItemCaseSensitive(json, "error");
        if (cJSON_IsString(error) && (error->valuestring != NULL)) {
            printf("Error: %s\n", error->valuestring);
        } else {
            // Assume the response is successful and print the book details
            cJSON *id = cJSON_GetObjectItemCaseSensitive(json, "id");
            cJSON *title = cJSON_GetObjectItemCaseSensitive(json, "title");
            cJSON *author = cJSON_GetObjectItemCaseSensitive(json, "author");
            cJSON *publisher = cJSON_GetObjectItemCaseSensitive(json, "publisher");
            cJSON *genre = cJSON_GetObjectItemCaseSensitive(json, "genre");
            cJSON *page_count = cJSON_GetObjectItemCaseSensitive(json, "page_count");

            printf("Book Details:\n");
            printf("id: %d\n", id->valueint);
            printf("title: %s\n", title->valuestring);
            printf("author: %s\n", author->valuestring);
            printf("publisher: %s\n", publisher->valuestring);
            printf("genre: %s\n", genre->valuestring);
            printf("page_count: %d\n", page_count->valueint);
        }

        cJSON_Delete(json);
    }
}
int main() {
    int loggedIn = 0;
    int enteredLibrary = 0;
    char *token = NULL;
    char *cookie = NULL;
    while (1) {
        char temp;
        char command[50];
        char *servResponse = NULL;

        printf("Command=");
        scanf("%[^\n]", command);
        scanf("%c",&temp);

        if (strcmp(command, "register") == 0) {
            if (loggedIn) {
                printf("ERROR: You are logged in! Please log out first to register a new account");
                continue;
            }
            char username[100], password[100];
            printf("username=");
            scanf("%[^\n]", username);
            scanf("%c",&temp);
            printf("password=");
            scanf("%[^\n]", password);
            scanf("%c",&temp);
            if (strchr(username, ' ') != NULL) {
                printf("ERROR: The username should not contain spaces!\n");
                continue;
            }
            if (strchr(password, ' ') != NULL) {
                printf("ERROR: The password should not contain spaces!\n");
                continue;
            }

            cJSON *root = cJSON_CreateObject();
            cJSON_AddStringToObject(root, "username", username);
            cJSON_AddStringToObject(root, "password", password);

            char *jsonPayload = cJSON_PrintUnformatted(root);
            char *servResponse = sendPostMessage(jsonPayload, "/api/v1/tema/auth/register", NULL);
            handle_json_response_register(servResponse);

            cJSON_Delete(root);
            free(jsonPayload);
        } else if (strcmp(command, "login") == 0) {
            if (loggedIn) {
                printf("ERROR: You are already logged into an account!\n");
                continue;
            }
            char username[100], password[100];
            printf("username=\n");
            scanf("%[^\n]", username);
            scanf("%c",&temp);
            printf("password=\n");
            scanf("%[^\n]", password);
            scanf("%c",&temp);
            if (strchr(username, ' ') != NULL) {
                printf("ERROR: The username should not contain spaces!\n");
                continue;
            }
            if (strchr(password, ' ') != NULL) {
                printf("ERROR: The password should not contain spaces!\n");
                continue;
            }
            cJSON *root = cJSON_CreateObject();
            cJSON_AddStringToObject(root, "username", username);
            cJSON_AddStringToObject(root, "password", password);

            char *jsonPayload = cJSON_PrintUnformatted(root);
            char *servResponse = sendPostMessage(jsonPayload, "/api/v1/tema/auth/login", NULL);
            if(handle_json_response_login(servResponse)){
                cookie = strdup(extract_cookie(servResponse));
                loggedIn = 1;
            }

        } else if (strcmp(command, "enter_library") == 0) {
            if (!loggedIn) {
                printf("ERROR: You are not logged into an account!\n");
                continue;
            }
            servResponse = sendGetMessage("/api/v1/tema/library/access", cookie, NULL);
            printf("SUCCESS: Successfully entered the library!\n");
            cJSON *json = cJSON_Parse(extract_json_body(servResponse));

            token = strdup(cJSON_GetObjectItemCaseSensitive(json, "token")->valuestring);
            enteredLibrary = 1;
        } else if (strcmp(command, "add_book") == 0) {
            int errors = 0;
            if (!(enteredLibrary && loggedIn)) {
                printf("ERROR: You need to be logged in and have entered the library to add a book!\n");
                continue;
            }
            char title[100], author[100], genre[100], page_count[100], publisher[100];
            printf("title=");
            scanf("%[^\n]", title);
            scanf("%c",&temp);
            printf("author=");
            scanf("%[^\n]", author);
            scanf("%c",&temp);

            if (isNumber(author)) {
                printf("ERROR: The Author needs to be a string!\n");
                errors = 1;
            }
            printf("genre=");
            scanf("%[^\n]", genre);
            scanf("%c",&temp);
            if (isNumber(genre)) {
                printf("ERROR: The Genre needs to be a string!\n");
                errors = 1;
            }
            printf("publisher=");
            scanf("%[^\n]", publisher);
            scanf("%c",&temp);
            if (isNumber(publisher)) {
                printf("ERROR: The publisher needs to be a string!\n");
                errors = 1;
            }

            printf("page_count=");
            scanf("%[^\n]", page_count);
            scanf("%c",&temp);
            if (!isNumber(page_count)) {
                printf("ERROR: Page count should be numeric!\n");
                errors = 1;
            }

            if(errors == 1){
                continue;
            }
            cJSON *root = cJSON_CreateObject();
            cJSON_AddStringToObject(root, "title", title);
            cJSON_AddStringToObject(root, "author", author);
            cJSON_AddStringToObject(root, "publisher", publisher);
            cJSON_AddStringToObject(root, "genre", genre);
            cJSON_AddStringToObject(root, "page_count", page_count);
            char *jsonPayload = cJSON_PrintUnformatted(root);
            sendPostMessage(jsonPayload, "/api/v1/tema/library/books", token);
            printf("SUCCESS: Book added successfully!\n");
        } else if (strcmp(command, "get_books") == 0) {
            if (!(enteredLibrary && loggedIn)) {
                printf("ERROR: You need to be logged in and have entered the library to add a book!\n");
                continue;
            }
            servResponse = sendGetMessage("/api/v1/tema/library/books", cookie, token);

            printf("SUCCESS: %s\n", extract_json_body(servResponse));
        } else if (strcmp(command, "get_book") == 0) {
            if (!(enteredLibrary && loggedIn)) {
                printf("ERROR: You need to be logged in and have entered the library to get a book!\n");
                continue;
            }
            char url[10000];
            char id[100];

            printf("id=");
            scanf("%[^\n]", id);
            scanf("%c",&temp);
            snprintf(url, sizeof(url), "/api/v1/tema/library/books/%s", id);
            servResponse = sendGetMessage(url, cookie, token);
            handle_book_response(extract_json_body(servResponse));   // Process the response

        } else if (strcmp(command, "delete_book") == 0) {
            if (!(enteredLibrary && loggedIn)) {
                printf("ERROR: You need to be logged in and have entered the library to add a book!\n");
                continue;
            }
            char url[10000];
            char id[100];
            printf("id=");
            scanf("%[^\n]", id);
            scanf("%c",&temp);
            snprintf(url, sizeof(url), "/api/v1/tema/library/books/%s", id);
            sendDeleteMessage(url, token);
            printf("SUCCESS: Book deleted successfully!\n");
        } else if (strcmp(command, "logout") == 0) {
            if(!loggedIn) {
                printf("ERROR: You are not logged in!\n");
                continue;
            }
            loggedIn = 0;
            enteredLibrary = 0;
            free(token);
            free(cookie);
            printf("SUCCESS: Successfully logged out!\n");
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            printf("Invalid command %s!\n", command);
        }
    }
    return 0;
}
