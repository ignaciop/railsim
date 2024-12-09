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

void print_summary(const double probability, const struct r_time *end, const struct r_time *start, const struct control *c) {
    struct r_time *et = delta_time(end, start);
    
    int tp = c->l1_passed_trains + c->l2_passed_trains + c->l3_passed_trains + c->l4_passed_trains;
    int q_avg = c->queued_acc / c->overloads;
    
    print_title();
    printf("\t%s%s%s\n", BOLD_FACE, "     | RUN SUMMARY |", RESET_COLOR);

    print_params(probability, c->rounds);
    
    printf("%s%s%02d:%02d:%02d%s\n", BOLD_FACE, "Elapsed Time: ", et->hour, et->min, et->sec, RESET_COLOR);
    
    printf("\n%s%s%s%s%d%s%s\n", PASSING_SIGN, RESET_COLOR, BOLD_FACE, "    | Total: ", tp, " |", RESET_COLOR);
    printf("\n%s%s%s%s%s%s%d%s%s\n", BOLD_FACE, LINE1_SIGN, " (A " , LEFT_ARROW_ICON, RIGHT_ARROW_ICON, " E)    | Total: ", c->l1_passed_trains, " |", RESET_COLOR);
    printf("%s%s%s%s%s%s%d%s%s\n", BOLD_FACE, LINE2_SIGN, " (A " , LEFT_ARROW_ICON, RIGHT_ARROW_ICON, " F)    | Total: ", c->l2_passed_trains, " |", RESET_COLOR);
    printf("%s%s%s%s%s%s%d%s%s\n", BOLD_FACE, LINE3_SIGN, " (B " , LEFT_ARROW_ICON, RIGHT_ARROW_ICON, " E)    | Total: ", c->l3_passed_trains, " |", RESET_COLOR);
    printf("%s%s%s%s%s%s%d%s%s\n", BOLD_FACE, LINE4_SIGN, " (B " , LEFT_ARROW_ICON, RIGHT_ARROW_ICON, " F)    | Total: ", c->l4_passed_trains, " |", RESET_COLOR);
    printf("\n%s%s%s%s%d%s%d%s%s\n", OVERLOAD_SIGN, RESET_COLOR, BOLD_FACE, "   | Total: ", c->overloads, " | Average number of trains waiting passage: ", q_avg, " |", RESET_COLOR);
    printf("\n%s%s%s%s%d%s%s\n\n", BREAKDOWN_SIGN, RESET_COLOR, BOLD_FACE, "   | Total: ", c->breakdowns, " |", RESET_COLOR);
    
    delete_time(&et);
}