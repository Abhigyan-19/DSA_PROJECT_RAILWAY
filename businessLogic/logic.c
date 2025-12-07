# include <logic.h>
# include "DSA_PROJECT_RAILWAY\Passengers\passenger.h"
# include "DSA_PROJECT_RAILWAY\Database\csv.h"
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


