#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "r_train.h"
#include "sg_queue.h"

#define TRAINS 5

int main(void) {
    srand(time(NULL));
    /*
    struct sg_queue *tq = sg_queue_new();
    
    for (int i = 0; i < TRAINS; i++) {
        struct train *t = new_train();
        
        sg_queue_enqueue(tq, (void *)t);
    }
    
    for (int i = 0; i < TRAINS; i++) {
        struct train *t = (struct train *)sg_queue_dequeue(tq);
        
        print_train(t);
        
        delete_train(&t);
    }
    
    sg_queue_delete(&tq);
    */
    char *messages[8] = {
        "\033[1;92m\U0001F781 Tunnel Passing \U0001F783\033[0m\033[1;97m   | \U0001F535 Line 1 (A \U0001F872 E) | \U0001F687 [T28] | \U0001F557 TA: 18:21:23 | \U0001F557 TD: 18:21:27 |\033[0m",
        "\033[1;93m\U0001F7B2 System Overload \U0001F7B2\033[0m\033[1;97m  | \U0001F557 TSO: 18:21:28 | \U0001F6A5 8 Trains Waiting Passage |\033[0m",
        "\033[1;91m\U0001F7AC Tunnel Breakdown \U0001F7AC\033[0m\033[1;97m | \U0001F535 Line 1 (A \U0001F872 E) | \U0001F687 [T22] | \U0001F557 TB: 18:21:44 | \U0001F6A5 6 trains waiting passage |\033[0m",
        "\033[1;92m\U0001F781 Tunnel Passing \U0001F783\033[0m\033[1;97m   | \U0001F7E3 Line 2 (A \U0001F870 F) | \U0001F687 [[T6]] | \U0001F557 TA: 18:22:01 \U0001F557 | TD: 18:22:04 |\033[0m",
        "\033[1;92m\U0001F781 Tunnel Passing \U0001F783\033[0m\033[1;97m   | \U0001F534 Line 3 (B \U0001F870 E) | \U0001F687 [T15] | \U0001F557 TA: 18:22:15 | \U0001F557 TD: 18:21:18 |\033[0m",
        "\033[1;92m\U0001F781 Tunnel Passing \U0001F783\033[0m\033[1;97m   | \U0001F7E0 Line 4 (B \U0001F872 F) | \U0001F687 [[T7]] | \U0001F557 TA: 18:23:10 | \U0001F557 TD: 18:23:12 |\033[0m",
        "\033[1;93m\U0001F7B2 System Overload \U0001F7B2\033[0m\033[1;97m  | \U0001F557 TSO: 18:23:14 | \U0001F6A5 7 Trains Waiting Passage |\033[0m",
        "\033[1;92m\U0001F781 Tunnel Passing \U0001F783\033[0m\033[1;97m   | \U0001F7E0 Line 4 (B \U0001F870 F) | \U0001F687 [T19] | \U0001F557 TA: 18:24:03 | \U0001F557 TD: 18:24:05 |\033[0m"
    };
    
    for (int i = 0; i < TRAINS * TRAINS; i++) {
        int index = rand() % 8;
        
        printf("%s\n", messages[index]);
        
        //sleep(1);
    }
    
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
    printf("\n%s\n", "\033[1;91m\U0001F7AC Tunnel Breakdowns \U0001F7AC\033[0m\033[1;97m  | Total: 4 | Average Number of Trains Waiting Passage: 5 |\033[0m");
    printf("\n%s\n\n", "\033[1;96m\U00002BC1 Tunnel Cleared \U00002BC1\033[0m\033[1;97m     | Time: 30 seconds |\033[0m");
    
    
    return EXIT_SUCCESS;
}