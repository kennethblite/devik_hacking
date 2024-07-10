#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simulating an asynchronous event
void on_event(char *data) {
    // Attacker can control what happens during this event
    printf("Event triggered: %s\n", data);
    strcpy(data, "Attacker controlled data");
}

void trigger_event(void (*event_handler)(char *), char *data) {
    // In a real program, this might be a network packet received or a timer event
    event_handler(data);
}

typedef struct {
    char *data;
} DataHolder;

void use_after_free() {
    // Allocate memory for the structure
    DataHolder *holder = (DataHolder *)malloc(sizeof(DataHolder));
    
    // Allocate memory for the data and assign it to the structure
    holder->data = (char *)malloc(100);
    strcpy(holder->data, "This is some initial data");

    // Print the data
    printf("Data before free: %s\n", holder->data);

    // Free the memory
    free(holder->data);
    // Now holder->data points to freed memory

    // Trigger an asynchronous event
    // This simulates the attacker taking control
    trigger_event(on_event, holder->data);

    // Accessing holder->data after the event
    printf("Data after free: %s\n", holder->data);

    // Free the remaining allocated memory
    free(holder);
}

int main() {
    use_after_free();
    return 0;
}

