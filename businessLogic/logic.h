#ifndef LOGIC_H
#define LOGIC_H

/* Global counters (declared extern for access in main.c) */
extern int maxSeats;
extern int Confirmed;
extern int Waiting;

/* Next passenger ID (auto-generated) */
extern int nextId;

/* Function prototypes */
int addPassenger(const char *name, int age, const char *src, const char *dest, const char *berth);
int cancelPassenger(int id);
int moveWLToConfirm(void);

#endif
