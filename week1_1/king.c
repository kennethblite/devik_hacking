#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int king;
    char *msg;
} RoyalMessage;

void use_after_free() {
    RoyalMessage *royal = (RoyalMessage *)malloc(sizeof(RoyalMessage));
    
    royal->msg = (char *)malloc(100);
    royal->king = 1;
    strcpy(royal->msg, "all of you are peasants");

    printf("King's message: %s\n", royal->msg);
    free(royal->msg);

    char peasant_reply[255];
    printf("Peasant reply: ");
    fgets(peasant_reply, sizeof(peasant_reply), stdin);

    char *new_message = (char *)malloc(strlen(peasant_reply) + 1);
    strcpy(new_message, peasant_reply);

    if (strstr(royal->msg, "I leave my flag to you")) {
        printf("Flag: FLAG-1234-SECRET\n");
    }else{
    	printf("King's response: %s\n", royal->msg);
    }

    // Free the remaining allocated memory
    free(new_message);
    free(royal);
}

int main() {
    use_after_free();
    return 0;
}

