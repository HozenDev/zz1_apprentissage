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
    (void) score;
    create_seed(&seed);

    rules_read_path_file("../data/rules/rules_balayage.txt", brain);
    
    enable_log(argc, argv);

    if (argc > 2)
    {
        if (strcmp(argv[2], "--load") == 0)
        {
            load_seed(&seed);
            save = 0;
            zlog(stdout, INFO, "chargement de la seed: %ld", seed);
        }
        else if (strcmp(argv[2], "--parallel") == 0)
        {
            rules_read_path_file("../data/rules/rules_balayage.txt", brain);
            zlog(stdout, INFO, "Brain read from file", 0);
            parallel_multiple_simulation(brain, &iter);
            rules_save_path_file("../data/rules/rules_balayage.txt", brain);
            zlog(stdout, INFO, "Brain save in file", 0);
        }
    }
    
    if (save) save_seed(seed);

    srand(seed);

    /* fprintf(stdout, "simulation fini: iter %d\n", iter); */

    /* parallel_loop(); */
    game_loop(brain, &iter);
    /* simulation_loop(brain, &iter); */
    
    //genetic_solve_optimized("../data/rules/rules_test.txt", "../data/rules/best_brain_genetic.txt");

    /* resolution_recuis_simule(&utils_descente_geometrique, "../data/rules/rules_test.txt", "../data/rules/rules_recuis_geometrique.txt", &score); */


    /* resolution_recuis_simule(&utils_descente_lineaire, "../data/rules/rules_test.txt", NULL, &score); */
    /* zlog(stdout, DEBUG ,"descente lineaire score %d\n", score); */
    
    return 0;
}
