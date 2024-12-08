#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "r_common.h"
#include "r_symbols.h"

void process_input(int argc, char *argv[], double *prob_arrive, int *rounds) {
    int opt;
    
    while ((opt = getopt(argc, argv, "p:r:")) != -1) {
        switch (opt) {
            case 'p':
                *prob_arrive = atof(optarg);
                
                break;
            
            case 'r':
                *rounds = atoi(optarg);
                
                break;
                
            default:
                break;
        }
    }
    
    if (*prob_arrive < SECTION_PROB_ARRIVE_MIN || *prob_arrive > SECTION_PROB_ARRIVE_MAX || *rounds < CONTROL_ROUNDS_MIN) {
        printf("%s\n\n%s%s%s%s\n\t%s%.1f%s%.1f%s\n\t%s%d%s\n\n%s\n", "Missing or incorrect input value(s)", "Usage: ", BOLD_FACE, "./railsim -p X -r N", RESET_COLOR, "X: Probability of trains arriving at each section every second (", SECTION_PROB_ARRIVE_MIN, " <= X <= ", SECTION_PROB_ARRIVE_MAX, ")", "N: Number of tunnel cleared rounds (N >= ", CONTROL_ROUNDS_MIN, ")", "Example with standard values: ./railsim -p 0.5 -r 5");
                
        exit(EXIT_FAILURE);
    }
}

void print_title(void) {
    printf("\n\t%s%s\n%s\n%s\n%s%s\n", BOLD_FACE, TITLE, BORDER, SUBTITLE, BORDER, RESET_COLOR);
}

void print_params(const double prob_arrive, const int rounds) {
    printf("\n%s%s\n\t\t%s%.1f%s\n\t\t%s%d%s%s\n\n", BOLD_FACE, "Chosen parameters:", "  X = ", prob_arrive,  " (probability of trains arriving at each section every second)", "  N = ", rounds, " (number of tunnel cleared rounds)", RESET_COLOR);
}

void print_summary(const struct control *c) {
 /*
    printf("\n%s", "\033[1;97m --------------------\033[0m");
    printf("\n%s", "\033[1;97m| Simulation Summary |\033[0m");
    printf("\n%s\n", "\033[1;97m --------------------\033[0m");
    printf("\n%s\n", "\033[1;95m\U00002BC1 Tunnel Clearings \U00002BC1\033[0m\033[1;97m    | Total: 2 | Average time taken: 00:01:10 |\033[0m");
    printf("\n%s\n", "\033[1;92m\U0001F781 Tunnel Passings \U0001F783\033[0m\033[1;97m    | Total: 81 |\033[0m");
    printf("%s\n", "\033[1;97m\U0001F535 Line 1 (A \U0001F870\U0001F872 E)     | Passing trains: 25 |\033[0m");
    printf("%s\n", "\033[1;97m\U0001F7E3 Line 2 (A \U0001F870\U0001F872 F)     | Passing trains: 19 |\033[0m");
    printf("%s\n", "\033[1;97m\U0001F534 Line 3 (B \U0001F870\U0001F872 E)     | Passing trains: 22 |\033[0m");
    printf("%s\n", "\033[1;97m\U0001F7E0 Line 4 (B \U0001F870\U0001F872 F)     | Passing trains: 15 |\033[0m");
    printf("\n%s\n", "\033[1;93m\U0001F7B2 System Overloads \U0001F7B2\033[0m\033[1;97m   | Total: 3 | Average number of trains waiting passage: 7 |\033[0m");
    printf("\n%s\n\n", "\033[1;91m\U0001F7AC Train Breakdowns \U0001F7AC\033[0m\033[1;97m  | Total: 4 |\033[0m");
    */
}