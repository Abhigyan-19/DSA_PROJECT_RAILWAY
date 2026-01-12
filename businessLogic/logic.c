#include "logic.h"
#include "../Passengers/passenger.h"
#include "../Database/csv.h"

#include <stdio.h>
#include <string.h>

/* Initialize counters */
int maxSeats = 10;
int Confirmed = 0;
int Waiting = 0;

/* Next passenger ID to assign */
int nextId = 1;

/* CSV file path */
const char *csvFile = "Database/passengers.csv";

/* Add Passenger */
int addPassenger(const char *name, int age, const char *src, const char *dest, const char *berth)
{
    int id = nextId++;  // Auto-generate ID

    Passenger *newPas;

    if (Confirmed < maxSeats) {
        newPas = create_Pass(id, age, name, src, dest, berth, "CONFIRMED");
        insert_Pass(&confirmedHead, newPas);
        Confirmed++;
        printf("Ticket confirmed for passenger %d\n", id);
    } else {
        newPas = create_Pass(id, age, name, src, dest, berth, "WL");
        insert_Pass(&waitingHead, newPas);
        Waiting++;
        printf("No seats available. Passenger %d added to waiting list.\n", id);
    }

    /* Save to CSV */
    saveData(confirmedHead, waitingHead, csvFile);

    return id;  // Return the generated ID
}

/* Cancel Passenger */
int cancelPassenger(int id)
{
    Passenger *p = search_Pass(confirmedHead, id);

    if (!p) {
        printf("Passenger ID %d not found in confirmed list.\n", id);
        return 0;
    }

    delete_Pass(&confirmedHead, id);
    Confirmed--;

    moveWLToConfirm();

    /* Save to CSV */
    saveData(confirmedHead, waitingHead, csvFile);

    printf("Passenger %d cancelled successfully.\n", id);
    return 1;
}

/* Move Waiting List Passenger to Confirmed */
int moveWLToConfirm(void)
{
    if (!waitingHead || Confirmed >= maxSeats) return 0;

    Passenger *p = waitingHead;

    delete_Pass(&waitingHead, p->id);
    Waiting--;

    strcpy(p->status, "CONFIRMED");

    insert_Pass(&confirmedHead, p);
    Confirmed++;

    printf("Waiting passenger %d promoted to confirmed.\n", p->id);

    /* Save to CSV */
    saveData(confirmedHead, waitingHead, csvFile);

    return 1;
}
