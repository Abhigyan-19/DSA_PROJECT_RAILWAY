#ifndef PASSENGER_H
#define PASSENGER_H

/* Passenger structure */
typedef struct Passenger {
    int id;
    int age;
    char name[50];
    char src[20];
    char dest[20];
    char berth[10];
    char status[20];
    struct Passenger *next;
} Passenger;

/* Global heads */
extern Passenger *confirmedHead;
extern Passenger *waitingHead;

/* Global counters */
extern int Confirmed;
extern int Waiting;

/* Function prototypes */

/* Create a new Passenger node */
Passenger *create_Pass(int id, int age, const char *name,
                       const char *src, const char *dest,
                       const char *berth, const char *status);

/* Insert passenger into circular linked list */
void insert_Pass(Passenger **head, Passenger *newPas);

/* Search passenger by ID */
Passenger *search_Pass(Passenger *head, int id);

/* Delete passenger by ID */
void delete_Pass(Passenger **head, int id);

/* Display all passengers in the list */
void display_Pass(Passenger *head);

#endif
