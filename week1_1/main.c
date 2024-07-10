#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    // This simulates the attacker allocating memory which could reuse the same location
    char *attacker_data = (char *)malloc(100);
    strcpy(attacker_data, "Attacker controlled data");

    // Print the data again, which will show the overwritten data
    printf("Data after free: %s\n", holder->data);

    // Free the remaining allocated memory
    free(attacker_data);
    free(holder);
}

int main() {
    use_after_free();
    return 0;
}

