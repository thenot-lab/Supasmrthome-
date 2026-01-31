#include "mutation.h"
#include <stdio.h>
#include <stdlib.h>

// Ingress module - Entry point and initialization
int main(int argc, char *argv[]) {
    // Initialize mutation engine with compile-time seed
    init_mutation(MUTATION_SEED);
    
    printf("Copilot 365 Agent - Build %u\n", MUTATION_SEED);
    
    // Initialize all modules
    init_propagation();
    init_targeting();
    init_effect();
    init_conceal();
    init_persistence();
    
    printf("All modules initialized successfully\n");
    
    return 0;
}
