#ifndef MUTATION_H
#define MUTATION_H

#include <stdint.h>

// Mutation configuration
#ifndef MUTATION_SEED
#define MUTATION_SEED 0
#endif

// Function declarations for mutation support
void init_mutation(uint32_t seed);
uint32_t get_mutation_value(void);

#endif // MUTATION_H
