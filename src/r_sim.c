#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "r_control.h"

int main(void) {
    srand(time(NULL));
    
    struct control *cc = new_control();
    
    pthread_t st[CONTROL_NUM_SECTIONS] = {0};
    pthread_t ct = 0;
    
    /*
     * cc->sections[0] represents Section AC
     * cc->sections[1] represents Section BC
     * cc->sections[2] represents Section DE
     * cc->sections[3] represents Section DF
     */
    for (int i = 0; i < CONTROL_NUM_SECTIONS; i++) {
        pthread_create(&st[i], NULL, add_train, (void *)(cc->sections[i]));
    }
    
    pthread_create(&ct, NULL, tunnel_control, (void *)cc);
    
    for (int i = 0; i < CONTROL_NUM_SECTIONS; i++) {
        pthread_join(st[i], NULL);
    }

    
    pthread_join(ct, NULL);
    
    delete_control(&cc);
   
    /*
    
    printf("%s\n\n", "\033[1;96m\U00002BC1 Tunnel Cleared \U00002BC1\033[0m\033[1;97m   | \U0001F557 TF: 18:24:06 | Time to Clear: 30 seconds |\033[0m");
    
    printf("\n%s", "\033[1;97m --------------------\033[0m");
    printf("\n%s", "\033[1;97m| Simulation Summary |\033[0m");
    printf("\n%s\n", "\033[1;97m --------------------\033[0m");
    printf("\n%s\n", "\033[1;92m\U0001F781 Tunnel Passings \U0001F783\033[0m\033[1;97m    | Total: 81 |\033[0m");
    printf("%s\n", "\033[1;97m\U0001F535 Line 1 (A \U0001F870\U0001F872 E)     | Passing Trains: 25 |\033[0m");
    printf("%s\n", "\033[1;97m\U0001F7E3 Line 2 (A \U0001F870\U0001F872 F)     | Passing Trains: 19 |\033[0m");
    printf("%s\n", "\033[1;97m\U0001F534 Line 3 (B \U0001F870\U0001F872 E)     | Passing Trains: 22 |\033[0m");
    printf("%s\n", "\033[1;97m\U0001F7E0 Line 4 (B \U0001F870\U0001F872 F)     | Passing Trains: 15 |\033[0m");
    printf("\n%s\n", "\033[1;93m\U0001F7B2 System Overloads \U0001F7B2\033[0m\033[1;97m   | Total: 3 | Average Number of Trains Waiting Passage: 7 |\033[0m");
    printf("\n%s\n\n", "\033[1;91m\U0001F7AC Tunnel Breakdowns \U0001F7AC\033[0m\033[1;97m  | Total: 4 | Average Number of Trains Waiting Passage: 5 |\033[0m");
    */
    
    return EXIT_SUCCESS;
}