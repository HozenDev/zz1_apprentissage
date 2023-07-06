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

    rules_read_path_file("../data/rules_test.txt", brain);
    
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
            rules_read_path_file("../data/rules_test.txt", brain);
            rules_save_file(stdout, brain);
            //parallel_multiple_simulation(brain, &iter);
            rules_save_path_file("../data/rules_test.txt", brain);
            rules_save_file(stdout, brain);
        }
    }
    
    if (save) save_seed(seed);

    srand(seed);
    
    rules_read_path_file("../data/hand_rules.txt", brain);
    //rules_save_file(stdout, brain);

    //parallel_multiple_simulation(brain, &iter);
    //rules_save_path_file("../data/rules_glouton.txt", brain);
    /* parallel_loop(); */
    //resolution_gloutone_aleatoire(brain, &iter); 
    /* zlog(stdout, INFO, "simulation fini: iter %d", iter); */
    
    //genetic_solve_optimized("../data/rules_test.txt", "../data/best_brain_genetic.txt");

    //resolution_recuis_simule(&utils_descente_geometrique, "../data/rules_test.txt", "../data/rules_recuis_geometrique.txt", &score);
    game_loop(brain, &iter);


    /* resolution_recuis_simule(&utils_descente_lineaire, "../data/rules_test.txt", NULL, &score); */
    /* zlog(stdout, DEBUG ,"descente lineaire score %d\n", score); */
    
    return 0;
}
