#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Passengers/passenger.h"
#include "Database/csv.h"
#include "businessLogic/logic.h"
#include "API/api_server.h"

/* Default settings */
#define DEFAULT_CSV_FILE "Database/passengers.csv"
#define DEFAULT_MAX_SEATS 10
#define DEFAULT_PORT "8000"

/* Global next ID for auto-generation */
extern int nextId ;  // Will be updated after loading CSV

/*
 * Entry point
 * Optional command-line arguments:
 *   ./backend [csv_file] [max_seats] [port]
 */
int main(int argc, char *argv[]) {

    const char *csvFile = (argc > 1) ? argv[1] : DEFAULT_CSV_FILE;
    maxSeats = (argc > 2) ? atoi(argv[2]) : DEFAULT_MAX_SEATS;
    const char *port = (argc > 3) ? argv[3] : DEFAULT_PORT;

    printf("=====================================\n");
    printf("   Train Reservation Backend Server  \n");
    printf("=====================================\n");

    printf("Loading data from CSV: %s\n", csvFile);
    loadData(&confirmedHead, &waitingHead, &nextId, csvFile); // pass nextId pointer and csv path

    printf("Data loaded successfully.\n");
    printf("Confirmed passengers : %d\n", Confirmed);
    printf("Waiting list         : %d\n", Waiting);
    printf("Next passenger ID    : %d\n", nextId);

    printf("Starting API server on port %s...\n", port);
    start_api_server(); // pass port as argument

    return 0;
}
