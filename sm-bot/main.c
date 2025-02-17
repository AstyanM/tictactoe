#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include <time.h>
void test(char* chaine, int temps);
void time_calculate();
int main(int argc, char* argv[]){
    
    // Recupère les arguments
    const char* coup = argv[1]; 
    const int temps = atoi(argv[2]);


    //printf("coup : %s et temps : %d", coup, temps);
    //time_calculate();
    //calculer le meilleur coup au jeu du super morpion
    /*const char *chaine = "6xoxOOOX2xo1ox1oXx2xo4oox4ox 84 o";*/


    test(coup, temps);


    return 0;

    
}
void test(char* chaine, int temps){
    supermopion sm;
    initialize(&sm);
    int last[2];
    int prochain[2] = {1,1};
    str_to_sm(chaine, &sm, last);  // transforme la chaine en supermorpion
    print_sm(sm);
    print_mp(get_large(sm));
    fprintf(stderr, "Last: %d %d\n", last[0], last[1]);
    fprintf(stderr,"Jouer: %c\n", sm.trait);
    meilleur_coup(sm, last, temps, prochain); // Test le meilleur coup
    printf("%d", prochain[0]*10+prochain[1]); // Renvoir le prochain coup
}

void time_calculate() {

    // BUT : Calculer le temps que prend une fonction à s'exécuter : pas utilisée 

    clock_t debut, fin;
    double temps_execution;
    supermopion sm;
    initialize(&sm);
    // Enregistrez le temps de début
    debut = clock();

    // Appelez la fonction à mesurer
    int k;
    for (k = 0; k<1000; k++) {
    
    scoup(&sm, 4, 5);
    score(sm);
    sm.morp[3].tableau[4]= infini;

    }
    // Enregistrez le temps de fin
    fin = clock();

    // Calculez le temps d'exécution en secondes
    temps_execution = ((double) (fin - debut)) / CLOCKS_PER_SEC;

    printf("Le temps d'execution est : %f secondes\n", temps_execution);

}