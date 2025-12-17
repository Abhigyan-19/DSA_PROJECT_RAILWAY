# include "logic.h"
# include "..\Passengers\passenger.h"
# include "..\Database\csv.h"
# include <stdio.h>


int maxSeats = 10;
int Confirmed = 0;
int Waiting = 0;
int addPassenger(int id, char *name, int age, char *src, char *dest, char *berth){
    if (Confirmed < maxSeats){
        Passenger *p = search_Pass (confirmedHead,id);
            if (p != NULL){
                printf ("ID already exists!\n"); 
                return 0;    
            }
        Passenger *newPas = create_Pass (id, age, name, src, dest, berth, "CONFIRMED!");
         insert_Pass(&confirmedHead, newPas);
        Confirmed ++;

    }
    else{
        Passenger *newPas = create_Pass (id, age, name, src, dest, berth, "WL");
        insert_Pass(&waitingHead, newPas);  
        Waiting ++;
        printf ("SORRY! We are getting inspired from Indigo. You are in the Waiting List");

    }
}

int cancelPassenger(int id) {

    // 1. Search in confirmed list
    Passenger *p = search_Pass(confirmedHead, id);

    if (p == NULL) {
        printf("Passenger ID not found in confirmed list.\n");
        return 0;
    }

    // 2. Remove the passenger from confirmed circular LL
    delete_Pass(&confirmedHead, id);
    Confirmed--;

    // 3. Move WL to Confirm if WL is not empty
    if (waitingHead != NULL) {
        moveWLToConfirm();
    }

    // 4. Save to CSV
    saveData(confirmedHead, waitingHead);

    printf("Passenger cancelled successfully.\n");
    return 1;
}

int moveWLToConfirm() {

    // 1. Check if WL is empty
    if (waitingHead == NULL) {
        return 0;   // No one to move
    }

    // 2. Pick the first WL passenger
    Passenger *p = waitingHead;

    // 3. Remove from WL
    delete_Pass(&waitingHead, p->id);
    Waiting--;

    // 4. Change status
    strcpy(p->status, "CONFIRMED");

    // 5. Insert into confirmed list
    insert_Pass(&confirmedHead, p);
    Confirmed++;

    // 6. Save to CSV
    saveData(confirmedHead, waitingHead);

    printf("Waiting list passenger %d moved to confirmed.\n", p->id);

    return 1;
}

int moveWLToConfirm() {

    // 1. Check if WL is empty
    if (waitingHead == NULL) {
        return 0;   // No one to move
    }

    // 2. Pick the first WL passenger
    Passenger *p = waitingHead;

    // 3. Remove from WL
    delete_Pass(&waitingHead, p->id);
    Waiting--;

    // 4. Change status
    strcpy(p->status, "CONFIRMED");

    // 5. Insert into confirmed list
    insert_Pass(&confirmedHead, p);
    Confirmed++;

    // 6. Save to CSV
    saveData(confirmedHead, waitingHead);

    printf("Waiting list passenger %d moved to confirmed.\n", p->id);

    return 1;
}



