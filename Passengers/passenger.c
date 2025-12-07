
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <passenger.h>

Passenger *confirmedHead = NULL; // Abhigyan- "ami ei line ta add korechi... reason meeting e bolbo"
Passenger *waitingHead = NULL;
const char *status = NULL;

Passenger *create_Pass (int id, int age, const char *name, const char *src, const char *dest,
                         const char *berth, const char *status)
{
        Passenger *p= (Passenger*)malloc (sizeof (Passenger));
        p-> id = id;
        p-> age = age;
        strcpy (p->name, name);
        strcpy (p->src, src);
        strcpy (p->dest, dest);
        strcpy (p->berth, berth);
        strcpy (p->status, status);

        p->next = p; 
        return p ;
}
void insert_Pass (Passenger **head, Passenger *newPas)
{
    if (*head == NULL)
    {
        *head = newPas;
        newPas -> next = newPas;
        return ;
    }
    Passenger *temp = head;
    while (temp->next != head)
        temp = temp-> next;
    temp->next = newPas;
    newPas->next = *head ;
}
Passenger *search_Pass (Passenger *head, int id)
{
    if (head == NULL)
        return;
    Passenger *temp = head;
    do
    {
        if (temp-> id == id)
            return temp;
        temp = temp-> next;
    }while (temp != head);
}
void display_Pass (Passenger *head)
{
    if (head == NULL)
    {
        printf ("Eveyone is ignoring your train just like INDIGO !!");
        return;
    }
    Passenger *temp = head;
    do 
    {
        printf ("%d |%s |%s ", temp->id, temp->name, temp->status );
        temp = temp->next;
    }while (temp != head); 
}


