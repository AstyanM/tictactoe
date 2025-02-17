#include <stdio.h>
#include <string.h>
#include "morpion.h"
#include <stdlib.h>
#include "utils.h"

int debug_mode = 0;


void play();
int main(int argc, char* argv[]){
    const int depth = atoi(argv[1]); // Récupération des arguments

    char* debug_env = getenv("DEBUG"); // Activation du mode DEBUG
    // Mode DEBUG activé si debug_env n'est pas NULL
    debug_mode = debug_env ? atoi(debug_env) : 0;
    fprintf(stderr, "debug_mode : %d", debug_mode);

    play(depth);
    return 0;
}

void play(int depth){


    supermopion sm;
    initialize(&sm);
    int prochain[2];
    int temps = depth;
    int last[2];
    int code=0;
    
    char str[100] = "999999999 00 o"; 

    str_to_sm(str, &sm, last); // Initialisation du supermorpion

    if (debug_mode) {
        printf("Initializing the game\n");
        }
    
    make_dot(chaine(sm));
    while(1){
        make_dot(str);

        if (!debug_mode) {
                system("clear");
        }
        

        if (debug_mode) {
        printf("DEBUG MODE ACTIVE\n");
        }

        printf("The condition in large is as follow:\n");
        print_mp(get_large(sm));
        printf("The detail is as follow:\n");
        print_sm(sm);

        switch(code){
        case -1:
            printf("The game has been won by %c !\n", sm.win);
            return;
        case -2:
            printf("Morpion %d is not in the range of 1 - 9 !\n", prochain[0]);
            break;
        case -3:
            printf("The morpion %d has been won by %c !\n", prochain[0], sm.morp[pos(prochain[0])].win);
            break;
        case -4:
            printf("Position %d is not in the range of 1 - 9 !\n", prochain[1]);
            break;
        case -5:
            printf("Position %d has been occupied by %c !\n", prochain[1], sm.morp[pos(prochain[0])].tableau[pos(prochain[1])]);
            break;
        case -6:
            printf("The position of last %d is not in the range of 1 - 9 !\n", last[1]);
            break;
        }
        /*==============================================*/
        if(sm.trait == 'x')  code = meilleur_coup(sm, last, temps, prochain);
        else code = get_coup(sm, last, temps, prochain);
        if(code < 0){
            continue;
        }
        scoup(&sm, prochain[0], prochain[1]); // On avance pour le prochain coup
        last[0] = prochain[0];
        last[1] = prochain[1];
        strcpy(str, chaine(sm));
    }
}
