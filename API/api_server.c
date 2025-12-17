#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mongoose.h"

#include "..\\Passengers\\passenger.h"
#include "..\\businessLogic\\logic.h"
#include "..\\Database\\csv.h"

// ===================================================
// JSON Conversion
// ===================================================
static void passenger_list_to_json(struct mg_connection *c, Passenger *head) {
    mg_printf(c, "[");

    Passenger *temp = head;
    int first = 1;

    while (temp != NULL) {
        if (!first) mg_printf(c, ",");
        first = 0;

        mg_printf(c,
            "{ \"id\": %d, \"age\": %d, \"name\": \"%s\", "
            "\"src\": \"%s\", \"dest\": \"%s\", "
            "\"berth\": \"%s\", \"status\": \"%s\" }",
            temp->id, temp->age, temp->name,
            temp->src, temp->dest, temp->berth, temp->status
        );

        temp = temp->next;
    }

    mg_printf(c, "]");
}

// ===================================================
// HANDLERS
// ===================================================
static void handle_create_node(struct mg_connection *c, struct mg_http_message *hm) {
    int id  = mg_json_get_long(hm->body, "$.id", 0);
    int age = mg_json_get_long(hm->body, "$.age", 0);

    char *name  = mg_json_get_str(hm->body, "$.name");
    char *src   = mg_json_get_str(hm->body, "$.src");
    char *dest  = mg_json_get_str(hm->body, "$.dest");
    char *berth = mg_json_get_str(hm->body, "$.berth");
    char *status = mg_json_get_str(hm->body, "$.status");

    Passenger *p = create_Pass(id, age, name, src, dest, berth, status);

    insert_Pass(&waitingHead, p);   // insert newly created passenger to WL

    free(name); free(src); free(dest); free(berth); free(status);

    mg_http_reply(c, 200, "", "{ \"status\": \"node_created\" }");
}

static void handle_insert(struct mg_connection *c, struct mg_http_message *hm) {
    int list = mg_json_get_long(hm->body, "$.list", 0);
    int id   = mg_json_get_long(hm->body, "$.id", 0);

    Passenger *p = search_Pass(list == 1 ? confirmedHead : waitingHead, id);

    if (!p) {
        mg_http_reply(c, 404, "", "{ \"error\": \"Passenger not found\" }");
        return;
    }

    if (list == 1)
        insert_Pass(&confirmedHead, p);
    else
        insert_Pass(&waitingHead, p);

    mg_http_reply(c, 200, "", "{ \"status\": \"inserted\" }");
}

static void handle_search(struct mg_connection *c, struct mg_http_message *hm) {
    char id_str[10];
    mg_http_get_var(&hm->query, "id", id_str, sizeof(id_str));

    int id = atoi(id_str);

    Passenger *p = search_Pass(confirmedHead, id);
    if (!p) p = search_Pass(waitingHead, id);

    if (!p) {
        mg_http_reply(c, 404, "", "{ \"status\": \"not_found\" }");
        return;
    }

    mg_http_reply(c, 200, "",
        "{ \"id\": %d, \"name\": \"%s\", \"status\": \"%s\" }",
        p->id, p->name, p->status
    );
}

static void handle_display(struct mg_connection *c) {
    mg_http_reply(c, 200, "Content-Type: application/json\r\n",
                  "{ \"confirmed\": ");
    
    passenger_list_to_json(c, confirmedHead);

    mg_printf(c, ", \"waiting\": ");
    passenger_list_to_json(c, waitingHead);

    mg_printf(c, " }");
}

static void handle_cancel(struct mg_connection *c, struct mg_http_message *hm) {
    int res = cancelPassenger();  // NO ARGUMENT — your logic.h says this

    if (res)
        mg_http_reply(c, 200, "", "{ \"status\": \"cancelled\" }");
    else
        mg_http_reply(c, 404, "", "{ \"status\": \"not_found\" }");
}

static void handle_moveWL(struct mg_connection *c) {
    int res = moveWLToConfirm();

    mg_http_reply(c, 200, "",
        res ? "{ \"status\": \"WL moved\" }" : "{ \"status\": \"WL empty\" }"
    );
}

// ===================================================
// ROUTER
// ===================================================
static void router(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev != MG_EV_HTTP_MSG) return;

    struct mg_http_message *hm = ev_data;

    if (mg_http_match_uri(hm, "/createPassengerNode")) handle_create_node(c, hm);
    else if (mg_http_match_uri(hm, "/insertPassenger")) handle_insert(c, hm);
    else if (mg_http_match_uri(hm, "/searchPassenger")) handle_search(c, hm);
    else if (mg_http_match_uri(hm, "/displayPassengers")) handle_display(c);
    else if (mg_http_match_uri(hm, "/cancel")) handle_cancel(c, hm);
    else if (mg_http_match_uri(hm, "/moveWL")) handle_moveWL(c);
    else mg_http_reply(c, 404, "", "{ \"error\": \"unknown_route\" }");
}

// ===================================================
// PUBLIC API
// ===================================================
void start_api_server() {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);

    printf("🚀 API Server running at: http://localhost:8080\n");

    mg_http_listen(&mgr, "http://localhost:8080", router, NULL);

    for (;;) mg_mgr_poll(&mgr, 1000);

    mg_mgr_free(&mgr);
}
