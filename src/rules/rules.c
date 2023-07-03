#include "rules.h"

void rules_save_file(FILE * file, char * path_file, rules_t * array_rules)
{
    (void) file;
    (void) path_file;
    (void) array_rules;
    //todo
}

void rules_init_file(FILE * file, char * path_file, rules_t * array_rules)
{
    (void) file;
    (void) path_file;
    (void) array_rules;
    // todo
}

void rules_copy_brain(rules_t * brainscr,rules_t * braindest)
{
    for (int i =0;i<NB_RULES;i++)
    {
        //copy measure
        for (int j=0;j<5;j++) braindest[i]->measures[j]=brainsrc[i]->measures[j];
        //copy action
        braindest[i]->action=brainsrc[i]->action;
        braindest[i]->priority=brainsrc[i]->priority;
    }
}
