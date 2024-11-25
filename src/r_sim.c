#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "r_section.h"
#include "r_control.h"


int main(void) {
    srand(time(NULL));
    
    double prob_arrive = 0.5;
    double prob_depart = 0.5;
    
    struct section *sect_AC = new_section('A', prob_arrive);
    //struct section *sect_DE = new_section('E', prob_arrive);
    //struct section *sect_BC = new_section('B', prob_arrive);
    //struct section *sect_DF = new_section('F', prob_arrive);
    
    pthread_t t_AC = 0;
    //pthread_t t_DE = 0;
    //pthread_t t_BC = 0;
    //pthread_t t_DF = 0;
    
    pthread_create(&t_AC, NULL, add_train, (void *)sect_AC);
    //pthread_create(&t_DE, NULL, add_train, (void *)sect_DE);
    //pthread_create(&t_BC, NULL, add_train, (void *)sect_BC);
    //pthread_create(&t_DF, NULL, add_train, (void *)sect_DF);
    
    pthread_t t_control = 0;
    
    pthread_create(&t_control, NULL, tunnel_control, (void *)sect_AC);
    
    
    pthread_join(t_AC, NULL);
    pthread_join(t_control, NULL);
    
    /*
    for (int i = 0; i < LOOPS; i++) {
        double p = (double)rand() / RAND_MAX;
        
        if (p < prob_arrive) {
            add_train(sect_AC);
            add_train(sect_DE);
            add_train(sect_DF);
        } else {
            add_train(sect_BC);
        }
    }
    
    printf("\nQueued Trains in AC: %d\n", sg_queue_size(sect_AC->trains));
    printf("\nQueued Trains in BC: %d\n", sg_queue_size(sect_BC->trains));
    printf("\nQueued Trains in DE: %d\n", sg_queue_size(sect_DE->trains));
    printf("\nQueued Trains in DF: %d\n\n", sg_queue_size(sect_DF->trains));
    
    while (1) {
        double p = (double)rand() / RAND_MAX;
        
        if (sg_queue_size(sect_AC->trains) != 0 || sg_queue_size(sect_BC->trains) != 0 || sg_queue_size(sect_DE->trains) != 0 || sg_queue_size(sect_DF->trains) != 0) {
            if (sg_queue_size(sect_AC->trains) != 0) {
                if (p < prob_depart) {
                    dispatch_train(sect_AC, 'E');
                    sleep(1);
                } else {
                    dispatch_train(sect_AC, 'F');
                    sleep(1);
                }
            }
            
            if (sg_queue_size(sect_BC->trains) != 0) {
                if (p < prob_depart) {
                    dispatch_train(sect_BC, 'E');
                    sleep(1);
                } else {
                    dispatch_train(sect_BC, 'F');
                    sleep(1);
                }
            }
            
            if (sg_queue_size(sect_DE->trains) != 0) {
                if (p < prob_depart) {
                    dispatch_train(sect_DE, 'A');
                    sleep(1);
                } else {
                    dispatch_train(sect_DE, 'B');
                    sleep(1);
                }
            }
            
            if (sg_queue_size(sect_DF->trains) != 0) {
                if (p < prob_depart) {
                    dispatch_train(sect_DF, 'A');
                    sleep(1);
                } else {
                    dispatch_train(sect_BC, 'B');
                    sleep(1);
                }
            }
        } else {
            break;
        }
    }
    
    printf("\nQueued Trains in AC: %d\n", sg_queue_size(sect_AC->trains));
    printf("\nQueued Trains in BC: %d\n", sg_queue_size(sect_BC->trains));
    printf("\nQueued Trains in DE: %d\n", sg_queue_size(sect_DE->trains));
    printf("\nQueued Trains in DF: %d\n\n", sg_queue_size(sect_DF->trains));

    delete_section(&sect_AC);
    delete_section(&sect_BC);
    delete_section(&sect_DE);
    delete_section(&sect_DF);
    */
    delete_section(&sect_AC);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    /*
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
    
    for (int i = 0; i < LINES * LINES; i++) {
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
    printf("\n%s\n\n", "\033[1;91m\U0001F7AC Tunnel Breakdowns \U0001F7AC\033[0m\033[1;97m  | Total: 4 | Average Number of Trains Waiting Passage: 5 |\033[0m");
    */
    
    return EXIT_SUCCESS;
}