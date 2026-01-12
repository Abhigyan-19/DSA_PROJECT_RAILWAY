#ifndef CSV_H
#define CSV_H

#include "../Passengers/passenger.h"

/* Load data from CSV into confirmed and waiting lists
 * Automatically updates nextId to max(existing IDs)+1
 */
void loadData(Passenger **confirmedHead, Passenger **waitingHead, int *nextId, const char *csvFile);

/* Save confirmed and waiting lists back to CSV */
void saveData(Passenger *confirmedHead, Passenger *waitingHead, const char *csvFile);

#endif
