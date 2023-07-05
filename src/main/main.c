#include "../log/log.h"
#include "../game/game.h"
#include "../seed/seed.h"
#include "../rules/rules.h"
#include "../genetic/genetic.h"
#include "../simulation/simulation.h"
#include "../parallel/parallel.h"

int main(int argc, char *argv[])
{
    int save = 1;
    long int seed;
    struct rules_s brain[NB_RULES];
    int iter=0;

    create_seed(&seed);
    
    enable_log(argc, argv);

    if (argc > 2)
    {
        if (strcmp(argv[2], "--load") == 0)
        {
            load_seed(&seed);
            save = 0;
            zlog(stdout, INFO, "chargement de la seed: %ld", seed);
        }
    }
    
    if (save) save_seed(seed);

    srand(seed);
    
    rules_read_path_file("../data/rules_test.txt", brain);
    rules_save_file(stdout, brain);

    /* parallel_loop(); */
    game_loop(brain, &iter);
    /* simulation_loop(brain, &iter); */
    zlog(stdout, INFO, "simulation fini: iter %d", iter);
    
    /* genetic_solve_optimized("../data/rules_test.txt", "../data/best_brain_genetic.txt"); */
    
    return 0;
}
