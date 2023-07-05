#include "../log/log.h"
#include "../game/game.h"
#include "../seed/seed.h"
#include "../rules/rules.h"
#include "../genetic/genetic.h"
#include "../simulation/simulation.h"
#include "../resolution/resolution.h"
#include "../utils/utils.h"
#include "../parallel/parallel.h"

int main(int argc, char *argv[])
{
    int save = 1;
    int iter=0;
    long int seed;
    struct rules_s brain[NB_RULES];
    int score;

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
    
    rules_read_path_file("../data/rules_init.txt", brain);
    rules_save_file(stdout, brain);

    parallel_multiple_simulation(brain, &iter);
    rules_save_path_file("../data/rules_test.txt", brain);
    /* parallel_loop(); */
    game_loop(brain, &iter);
    /* simulation_loop(brain, &iter); */
    /* zlog(stdout, INFO, "simulation fini: iter %d", iter); */
    
    /* genetic_solve_optimized("../data/rules_test.txt", "../data/best_brain_genetic.txt"); */

    /* resolution_recuis_simule(&utils_descente_geometrique, "../data/rules_test.txt", NULL, &score); */
    /* zlog(stdout, DEBUG ,"descente geometrique score %d\n", score); */

    /* resolution_recuis_simule(&utils_descente_lineaire, "../data/rules_test.txt", NULL, &score); */
    /* zlog(stdout, DEBUG ,"descente lineaire score %d\n", score); */
    
    return 0;
}
