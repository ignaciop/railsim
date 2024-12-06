#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "r_control.h"
#include "r_symbols.h"

int main(int argc, char *argv[]) {
    int opt = 0;
    
    double prob_arrive = -1.0;
    int sim_time = -1;
    
    while ((opt = getopt(argc, argv, "p:s:")) != -1) {
        switch (opt) {
            case 'p':
                prob_arrive = atof(optarg);
                
                break;
            
            case 's':
                sim_time = atoi(optarg);
                
                break;
                
            default:
                break;
        }
    }
    
    if (prob_arrive < SECTION_PROB_ARRIVE_MIN || prob_arrive > SECTION_PROB_ARRIVE_MAX || sim_time < CONTROL_SIM_TIME_MIN) {
        printf("%s\n\n%s%s%s%s\n\t%s%.1f%s%.1f%s\n\t%s%d%s\n\n%s\n", "Missing or incorrect input value(s)", "Usage: ", BOLD_FACE, "./railsim -p X -s N", RESET_COLOR, "X: Probability of trains arriving at each section every second (", SECTION_PROB_ARRIVE_MIN, " <= X <= ", SECTION_PROB_ARRIVE_MAX, ")", "N: Total simulation time in seconds (N >= ", CONTROL_SIM_TIME_MIN, ")", "Example with standard values: ./railsim -p 0.5 -s 60");
                
        exit(EXIT_FAILURE);
    }
    
    printf("\n\t%s%s\n%s\n%s\n%s%s\n", BOLD_FACE, TITLE, BORDER, SUBTITLE, BORDER, RESET_COLOR);
    
    printf("(p = %.1f, s = %d)\n", prob_arrive, sim_time);
    
    srand(time(NULL));
    
    struct control *cc = new_control(prob_arrive);
    
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