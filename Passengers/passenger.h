//#ifndef PASSENGER_H
//#define PASSENGER_H
//
//extern Passenger *confirmedHead; // Abhigyan - " ami add korechi... pore meeting e bolbo "
//extern Passenger *waitingHead;
//extern const char *status; // Abhigyan - "Same Logic"
//
//typedef struct Passenger
//{
//    int id;
//    int age;
//    char name[30];
//    char src[20];
//    char dest[20];
//    char berth [10];
//    char status[10];
//    struct Node *next;
//} Passenger;
//
//// Function declaration
//extern *Confirmed ;
//extern *Waiting ;
//
//Passenger *create_Pass (int id, int age, const char *name, const char *src, const char *dest,
//                         const char *berth, const char *status);
//void insert_Pass (Passenger **head, Passenger *newPas);
//Passenger *search_Pass (Passenger *head, int id);
//void display_Pass (Passenger *head);
//
//
//#endif

#ifndef PASSENGER_H
#define PASSENGER_H

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

// Global heads
extern Passenger *confirmedHead;
extern Passenger *waitingHead;

// Global counters
extern int Confirmed;
extern int Waiting;

// Function prototypes
Passenger *create_Pass(int id, int age, const char *name, const char *src, const char *dest,
                       const char *berth, const char *status);

void insert_Pass(Passenger **head, Passenger *newPas);

Passenger *search_Pass(Passenger *head, int id);

void delete_Pass(Passenger **head, int id);   // You MUST implement this!

void display_Pass(Passenger *head);

#endif
