#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "passenger.h"

/* Global heads */
Passenger *confirmedHead = NULL;
Passenger *waitingHead = NULL;

/* Create a new passenger */
Passenger *create_Pass(int id, int age, const char *name,
                       const char *src, const char *dest,
                       const char *berth, const char *status)
{
    Passenger *p = (Passenger *)malloc(sizeof(Passenger));
    if (!p) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    p->id = id;
    p->age = age;
    strncpy(p->name, name, sizeof(p->name)-1);
    p->name[sizeof(p->name)-1] = '\0';

    strncpy(p->src, src, sizeof(p->src)-1);
    p->src[sizeof(p->src)-1] = '\0';

    strncpy(p->dest, dest, sizeof(p->dest)-1);
    p->dest[sizeof(p->dest)-1] = '\0';

    strncpy(p->berth, berth, sizeof(p->berth)-1);
    p->berth[sizeof(p->berth)-1] = '\0';

    strncpy(p->status, status, sizeof(p->status)-1);
    p->status[sizeof(p->status)-1] = '\0';

    p->next = NULL;  // Will be linked in insert_Pass

    return p;
}

/* Insert passenger into circular linked list */
void insert_Pass(Passenger **head, Passenger *newPas)
{
    if (!head || !newPas) return;

    if (*head == NULL) {
        *head = newPas;
        newPas->next = newPas;
        return;
    }

    Passenger *temp = *head;
    while (temp->next != *head)
        temp = temp->next;

    temp->next = newPas;
    newPas->next = *head;
}

/* Search passenger by ID */
Passenger *search_Pass(Passenger *head, int id)
{
    if (!head) return NULL;

    Passenger *temp = head;
    do {
        if (temp->id == id)
            return temp;
        temp = temp->next;
    } while (temp != head);

    return NULL;
}

/* Delete passenger by ID */
void delete_Pass(Passenger **head, int id)
{
    if (!head || !*head) return;

    Passenger *curr = *head;
    Passenger *prev = NULL;

    /* Single node case */
    if (curr->next == curr && curr->id == id) {
        free(curr);
        *head = NULL;
        return;
    }

    /* Traverse list to find node */
    do {
        if (curr->id == id)
            break;
        prev = curr;
        curr = curr->next;
    } while (curr != *head);

    if (curr->id != id)
        return; // Not found

    /* Deleting head node */
    if (curr == *head) {
        Passenger *last = *head;
        while (last->next != *head)
            last = last->next;

        *head = curr->next;
        last->next = *head;
    } else {
        prev->next = curr->next;
    }

    free(curr);
}

/* Display list */
void display_Pass(Passenger *head)
{
    if (!head) {
        printf("No passengers found.\n");
        return;
    }

    Passenger *temp = head;
    do {
        printf("ID:%d | Name:%s | Status:%s | Src:%s | Dest:%s | Berth:%s | Age:%d\n",
               temp->id, temp->name, temp->status,
               temp->src, temp->dest, temp->berth, temp->age);
        temp = temp->next;
    } while (temp != head);
}
