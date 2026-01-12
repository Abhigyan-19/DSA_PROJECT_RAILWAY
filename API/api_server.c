#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <winsock2.h>   // Windows sockets
#endif
#define MG_ENABLE_HTTP 1  // Enable HTTP API before including mongoose

#include "mongoose.h"
#include "api_server.h"

#include "../businessLogic/logic.h"
#include "../Passengers/passenger.h"

/* Server port */
#define PORT "8000"

/* -------- BOOK / CANCEL / SEARCH HANDLER -------- */
static void handle_request(struct mg_connection *c, int ev, void *ev_data) {
    if (ev != MG_EV_HTTP_MSG) return;

    struct mg_http_message *hm = (struct mg_http_message *) ev_data;

    // CORS headers for all responses
    const char *cors_headers = 
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n";

    // Handle OPTIONS preflight request
    if (mg_strcmp(hm->method, mg_str("OPTIONS")) == 0) {
        mg_http_reply(c, 200, cors_headers, "");
        return;
    }

    /* -------- BOOK TICKET -------- */
    if (mg_strcmp(hm->uri, mg_str("/book")) == 0) {
        char name[50], src[20], dest[20], berth[10], age_buf[10];
        int age;

        mg_http_get_var(&hm->body, "name", name, sizeof(name));
        mg_http_get_var(&hm->body, "age", age_buf, sizeof(age_buf));
        age = atoi(age_buf);
        mg_http_get_var(&hm->body, "src", src, sizeof(src));
        mg_http_get_var(&hm->body, "dest", dest, sizeof(dest));
        mg_http_get_var(&hm->body, "berth", berth, sizeof(berth));

        int passengerId = addPassenger(name, age, src, dest, berth);

        // Search for the newly created passenger to get complete details
        Passenger *p = search_Pass(confirmedHead, passengerId);
        if (!p) p = search_Pass(waitingHead, passengerId);

        char response_headers[512];
        snprintf(response_headers, sizeof(response_headers), 
                 "%sContent-Type: application/json\r\n", cors_headers);

        if (p) {
            // Return complete passenger details after booking
            mg_http_reply(c, 200, response_headers,
                          "{ \"success\": %d, "
                          "\"id\": %d, "
                          "\"name\": \"%s\", "
                          "\"age\": %d, "
                          "\"src\": \"%s\", "
                          "\"dest\": \"%s\", "
                          "\"berth\": \"%s\", "
                          "\"status\": \"%s\" }\n",
                          passengerId, p->id, p->name, p->age,
                          p->src, p->dest, p->berth, p->status);
        } else {
            // Fallback if passenger not found (shouldn't happen)
            mg_http_reply(c, 200, response_headers,
                          "{ \"success\": %d }\n", passengerId);
        }
        return;
    }

    /* -------- CANCEL TICKET -------- */
    if (mg_strcmp(hm->uri, mg_str("/cancel")) == 0) {
        char buf[20];
        mg_http_get_var(&hm->body, "id", buf, sizeof(buf));
        int id = atoi(buf);

        int result = cancelPassenger(id);

        char response_headers[512];
        snprintf(response_headers, sizeof(response_headers), 
                 "%sContent-Type: application/json\r\n", cors_headers);

        mg_http_reply(c, 200, response_headers,
                      "{ \"success\": %d }\n", result);
        return;
    }

    /* -------- SEARCH PASSENGER -------- */
    if (mg_strcmp(hm->uri, mg_str("/search")) == 0) {
        char buf[20];
        mg_http_get_var(&hm->body, "id", buf, sizeof(buf));
        int id = atoi(buf);

        Passenger *p = search_Pass(confirmedHead, id);
        if (!p) p = search_Pass(waitingHead, id);

        char response_headers[512];
        snprintf(response_headers, sizeof(response_headers), 
                 "%sContent-Type: application/json\r\n", cors_headers);

        if (!p) {
            mg_http_reply(c, 404, response_headers,
                          "{ \"error\": \"Passenger not found\" }\n");
        } else {
            mg_http_reply(c, 200, response_headers,
                          "{ \"id\": %d, \"name\": \"%s\", \"age\": %d, "
                          "\"src\": \"%s\", \"dest\": \"%s\", "
                          "\"berth\": \"%s\", \"status\": \"%s\" }\n",
                          p->id, p->name, p->age,
                          p->src, p->dest, p->berth, p->status);
        }
        return;
    }

    /* -------- INVALID ROUTE -------- */
    char response_headers[512];
    snprintf(response_headers, sizeof(response_headers), 
             "%sContent-Type: text/plain\r\n", cors_headers);
    mg_http_reply(c, 404, response_headers, "Invalid API endpoint\n");
}

/* -------- START API SERVER -------- */
void start_api_server(void) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed\n");
        return;
    }
#endif

    struct mg_mgr mgr;
    mg_mgr_init(&mgr);

    char address[50];
    snprintf(address, sizeof(address), "http://0.0.0.0:%s", PORT);

    if (mg_http_listen(&mgr, address, handle_request, NULL) == NULL) {
        fprintf(stderr, "Error starting server on port %s\n", PORT);
        mg_mgr_free(&mgr);
#ifdef _WIN32
        WSACleanup();
#endif
        return;
    }

    printf("API Server running on port %s\n", PORT);

    /* Poll events indefinitely */
    for (;;) mg_mgr_poll(&mgr, 100);

    mg_mgr_free(&mgr);

#ifdef _WIN32
    WSACleanup();
#endif
}