#include "mutation.h"
#include <stdio.h>

// Propagation module - Network and spread functionality
static uint32_t mutation_state = 0;

void init_mutation(uint32_t seed) {
    mutation_state = seed;
}

uint32_t get_mutation_value(void) {
    return mutation_state;
}

void init_propagation(void) {
    printf("Propagation module initialized (seed: %u)\n", mutation_state);
}
