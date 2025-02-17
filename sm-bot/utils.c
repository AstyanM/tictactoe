#include <stdio.h>
#include "utils.h"
#include <time.h>
#define MAX 500

static clock_t debut = 18;
static clock_t fin = 180;


int saisie(){

    //BUT : Permet de saisir des coordonnées avec les nombres sur le coté du clavier
    
    int m;
    /*printf("Please choose a place(1-9) on the right number keyboard:");*/
    fprintf(stderr,"(right number keyboard): ");

    //1-7, 4-4, 7-1 9-3, 3-9
    scanf("%d", &m);
    if(m < 4){
        m += 6;
    }
    else if(m > 6){
        m -= 6;
    }
    return m;
}
int sasie2(){ 
    
    //BUT : Permet de saisir des coordonnées sous la forme (a-c)(1-3)
    
    int m;
    
    char place[2];
    scanf("%s", place);
    if(place[0] < 'a' || place[0] > 'c'){
        perror("Your place is out of the morpion ! defaut a !\n");
        place[0] = 'a';
    }
    if(place[1] < '1' || place[1] > '3'){
        perror("Your place is out of the morpion ! defaut 1 !\n");
        place[1] = '1';
    }
    m = ('3' - place[1]) * 3 + place[0] - 'a' + 1;   


    return m;
}


void temps_to_depth(const double temps){ 
    
    // BUT : Convertir le temps qui reste en profondeur à donner à l'algorithme

    char* filename = "depth.txt";
    FILE *file = fopen(filename, "r");

    int depth;

    // Fichier ouvert correctement ?
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return;
    }

    fscanf(file, "%d", &depth); // On prend la valeur depth sauvegardée dans le fichier "depth.txt"
    fclose(file);

    double temps_execution;

    temps_execution = ((double) (fin - debut)) / CLOCKS_PER_SEC; // On calcule le temps d'execution du programme au tour d'avant

    // On classe le temps fait précédement en fonction du temps alloué et et du temps restant 
    // Si le programme à fait un petit temps alors qu'il reste beaucoup de temps, c'est qu'il n'est pas allé assez loin, donc on augmente de 1 la depth)

    if (temps_execution > 30 && temps <= 300) {depth -= 1; // Trop long et temps restant < 5min -> |-1|
    fprintf(stderr, "depth (temps_execution > 30 && temps <= 300) : %d\n", depth);} 
    else if (temps_execution < 10 && temps <= 300) {depth += 1; // Trop cours et temps restant < 5min -> |+1|
    fprintf(stderr, "depth (temps_execution < 4 && temps <= 300) : %d\n", depth);} 
    else if (temps_execution > 90 && temps > 300 && temps <= 600) {depth -= 1; // Trop long et temps restant < 10min -> |-1|
    fprintf(stderr, "depth (temps_execution > 45 && temps > 300 && temps <= 600) : %d\n", depth);}
    else if (temps_execution < 20 && temps > 300 && temps <= 600) {depth += 1; // Trop court et temps restant < 10min -> |+1|
    fprintf(stderr, "depth (temps_execution < 5 && temps > 300 && temps <= 600): %d\n", depth);}
    else if (temps_execution > 120 && temps > 600) {depth -= 1; // Trop long et temps restant < 15min -> |-1|
    fprintf(stderr, "depth (temps_execution > 60 && temps > 600 && temps <= 900): %d\n", depth);}
    else if (temps_execution < 40 && temps > 600) {depth += 1; // Trop court et temps restant < 15min -> |+1|
    fprintf(stderr, "depth (temps_execution < 6 && temps > 600 && temps <= 900): %d\n", depth);}
    else { // Cas normal, on ne bouge rien -> |+0|
        depth += 0; 
        fprintf(stderr, "depth (normal): %d\n", depth);
    }

    FILE *file2 = fopen(filename, "w"); // On rentre la nouvelle valeur dans le fichier
    fprintf(file2, "%d", depth);

    fclose(file2);

    // Ancienne fonction qui calculait à l'avance la depth max possible pour un temps donnée grâce à une moyenne du temps pour calculer un supermorpion

        /*const float Dt = 0.000008626; 
        long double sum = 0; 
        long long int x = 1;
        while (1)
        {   
            x *= SIZE;
            sum = Dt*(x-1)/8;
            if (sum >= temps) {
                return depth;
            }
            depth++;  
            fprintf(stderr,"sum : %Lf and depth : %d\n", sum, depth);
        }*/

    fprintf(stderr, "depth : %d\n", depth);

}

int get_new_morpion(const supermopion sm, const int last);

char N(const char trait){ // Fonction pratique, couleur to number
    return trait == black ? white:black;
}
int M(int A, int B){ // Fonction pratique, comparaison
    return A>B? A:B;
}
int mcore(const morpion mp, const char trait){

    // BUT : calculer le score d'un morpion


    morpion try = mp;
    int s = 0;
    int maxs = 200;
    int t[][3] = { // Tableau des triplés gagnants
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };
    int a[3];
    for(int i=0;i<8;i++){ // Parcours des triplés gagnants
        for(int j = 0; j<3;j++){ // Parcours des cases du triplé gagnant
            if(mp.tableau[t[i][j]] == trait){ // C'est un pion à nous : on remplis la première case d'un 2
                a[j] = 2;
            }else if(mp.tableau[t[i][j]] == N(trait)){ // C'est un pion à l'adversaire : on remplis la première case d'un 3
                a[j] = 3;
            }else if(mp.tableau[t[i][j] == infini]){ // C'est une case vide : on remplis la première case d'un 5
                a[j] = 5;
            }

        }

        if(win(&try) == trait) return maxs; // Cas d'une victoire du joueur : SCORE maximal
        else if(win(&try) == N(trait)) return -maxs; // Cas d'une victoire de l'adversaire : SCORE minimal
        else if(a[0] * a[1] * a[2] == 20) s += 10; // Cas de deux pions du joueur sur le même triplé : SCORE + 10
        else if(a[0] * a[1] * a[2] == 45) s -= 10; // Cas de deux pions adverse sur le même triplé : SCORE - 10
        else if(a[0] * a[1] * a[2] == 50) s += 5; // Cas d'un pion du joueur sur le même triplé : SCORE + 5
        else if(a[0] * a[1] * a[2] == 75) s -= 5; // Cas d'un pion adverse sur le même triplé : SCORE - 5

        if(a[1] == 2) s -= 3; // Discrimination des cases côté pour le joueur (2,4,6,8): SCORE - 3
        else if(a[1] == 3) s += 3; // Discrimination des cases côté pour l'adversaire (2,4,6,8): SCORE + 3
        
        if (t[i][1] == 5 && a[1] == 2) s += 7; // Avantage joueur du centre de la case : SCORE + 7
        else if (t[i][1] == 5  && a[1] == 3) s -= 3; // Desavantage joueur du centre de la case : SCORE - 3
    }

    return s;
}
int score(const supermopion sm){
    
    // BUT : calculer le score d'un supermorpion

    int s = 0;

    morpion large = get_large(sm); // Transforme le supermorpion en morpion classique et le passe dans mscore (avec un + gros SCORE (x2))                                                                                           
    s += 2 * mcore(large, sm.trait);

    for(int m=1; m<=SIZE;m++){ // Rajoute les scores des morpions annexes
        morpion mp = sm.morp[pos(m)];
            s += mcore(mp, sm.trait); 
    }
    return s;
}

int abs(int a){ // Fonction pratique : Valeur absolue
    return a > 0? a: -a;
}
int negamax0(const supermopion sm, const int m, const int depth, int alpha, int beta){ 
    
    // BUT : Fonction Negamax0, non utilisée
    
    // m: current morpion, 1-9
    supermopion try = sm;
    if(depth == 0 || swin(&try) != infini){
        return score(sm);
    }
    int val = -MAX;
    for(int i=1;i<=SIZE; i++){
        if(sm.morp[m - 1].tableau[i - 1] != infini){
            continue;
        }
        scoup(&try, m, i);//contenir changer le trait
        int new_m = get_new_morpion(try, i);
        int limite = -negamax0(try, new_m, depth - 1, -beta, -alpha);
        // in the next morpion, caculate the best next morpion, and calculate its score, and then renvoie by the new_mscore, so we can got the next next mscore,
        // which should be higher

        /*if(sm.morp[pos(new_m)].win != infini){*/
        /*limite -= 5;//IMPORTANT: It's not good to let the adversaire to choose the morpion.*/
        /*//TODO: adjust the number*/
        /*}*/

        val = (val > limite)? val:limite; 
        if(val >= beta) break;
        alpha = M(alpha, val);
        try = sm;
    }
    return val;
}

int is_board_full(const morpion mp) {
    for (int i = 0; i < SIZE; i++) {
        if (mp.tableau[i] == infini) {
            return 0; // Il y a encore une case vide
        }
    }
    return 1; // Toutes les cases sont remplies
}

int negamax(const supermopion sm, const int m, const int depth, int alpha, int beta){

    // BUT : Fonction Negamax avec élagage Alpha Beta

    // m: current morpion, 1-9
    supermopion try = sm;
    if(depth == 0 || swin(&try) != infini){
        return score(sm);
    }
    int val = -MAX;
    for(int i=1;i<=SIZE; i++){
        if(sm.morp[m - 1].tableau[i - 1] != infini){
            continue;
        }
        scoup(&try, m, i); //contenir changer le trait
        int new_m = get_new_morpion(try, i);
        int sc = mcore(sm.morp[pos(new_m)], sm.trait); 
        int limite = -negamax(try, new_m, depth - 1, -beta, -alpha) - 2 * abs(sc);
        // in the next morpion, caculate the best next morpion, and calculate its score, and then renvoie by the new_mscore, so we can got the next next mscore,
        // which should be higher

        /*if(sm.morp[pos(new_m)].win != infini){*/
        /*limite -= 5;//IMPORTANT: It's not good to let the adversaire to choose the morpion.*/
        /*//TODO: adjust the number*/
        /*}*/

        val = (val > limite)? val:limite; 
        if(val >= beta) break;
        alpha = M(alpha, val);
        try = sm;
    }
    return val;
}
int get_new_morpion(const supermopion sm, const int last){

    // BUT : Prélever le morpion qui a la plus grande valeur

    // last: 1-9
    if(last > 0 && last <= SIZE && sm.morp[pos(last)].win == infini){
        return last;
    }
    if(last == 0){
        fprintf(stderr,"last = 0!\n");
    }
    /*if(sm.morp[pos(last)].win != infini){*/
    /*printf("the last %d has been remported by %c!\n", last, sm.morp[pos(last)].win);*/
    /*}*/

    int val=-MAX;
    int max_val_i = 0;
    for(int i = 1;i<=SIZE;i++){
        if(sm.morp[pos(i)].win != infini || is_board_full(sm.morp[pos(i)])){
            continue;
        }
        /*int l[2] = {0, i};*/
        /*int prochain[2];*/
        /*int limite = meilleur_coup(sm, l, 3, prochain); //TODO: temps*/
        int limite = mcore(sm.morp[pos(i)], sm.trait);
        val = (val > limite)? val: limite;
        if(val == limite){
            max_val_i = i;
        }
    }
    /*printf("new morpion is: %d\n", max_val_i);*/
    return max_val_i;
}


int meilleur_coup(const supermopion sm, const int last[], int temps, int prochain[]){

    // BUT : Calculer le meilleurs coup possible 

    supermopion try = sm;

    int m = get_new_morpion(sm, last[1]);

    int depth;
    char* filename = "depth.txt";
    FILE *file = fopen(filename, "r");

    // Fichier correctement ouvert
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return;
    }

    fscanf(file, "%d", &depth);

    fclose(file);

    double temps_execution;


    debut = clock(); // Debut de la clock pour calculer le temps du programme

    if(swin(&try) != infini || depth == 0){
        return -1;
    }
    prochain[0] = m; // 1-9
    int val = -MAX;
    int i=1;
    for(i=1; i<=SIZE;i++){
        if(sm.morp[pos(m)].tableau[pos(i)] != infini || is_board_full(sm.morp[pos(m)])){
            continue;
        }
        scoup(&try, m, i);
        int new_m = get_new_morpion(try, i);
        int limite = -negamax(try, new_m, depth - 1, -MAX, MAX);
        if(sm.morp[pos(i)].win != black){
            limite -= 10;//IMPORTANT: It's not good to let the adversaire to choose the morpion.
            //TODO: adjust the number
        }
        val = (val > limite)? val: limite;


        if(val == limite){
            prochain[1] = i; // 1-9
            
        }
        try = sm;
    }
    fin = clock(); // Fin de la clock pour calculer le temps du programme


    temps_to_depth(temps);
    return 0;
}
void str_to_sm(const char* s, supermopion* sm, int last[]){ 

    // BUT : traduire une ligne FEN en supermorpion

    int i = 1;
    int j = 1;
    char* c = s; 
    for(; *c != ' ' && i <= SIZE; c ++){
        if(*c == 'O' || *c == 'X'){
            /*sm->morp[pos(i)].win = *c - 'A' + 'a';*/
            sm->trait = *c - 'A' + 'a';
            scoup(sm, i, 1);
            sm->trait = *c - 'A' + 'a';
            scoup(sm, i, 5);
            sm->trait = *c - 'A' + 'a';
            scoup(sm, i, 9);
            i ++;
        }
        else if(*c <= '9' && *c >= '1'){
            for(int m=0;m<*c - '0' && j <= SIZE && i <= SIZE; m++){
                sm->morp[pos(i)].tableau[pos(j++)] = infini;
            }
        }
        else if(*c == black || *c == white){
            sm -> morp[pos(i)].tableau[pos(j++)] = *c;
        }
        /*if(j != 0)*/
        /*printf("%d, %d: %c\n", i, j-1, sm -> morp[i].tableau[j-1]);*/
        if(j > SIZE){
            j-=SIZE;
            i++;
        }
    }
    last[0] = *(++c) - '0';
    last[1] = *(++c) - '0';
    c+=2;
    sm -> trait = *c;
    swin(sm);
}


int get_coup(const supermopion sm, const int last[], int temps, int prochain[]){

    // BUT : Prendre le coup du joueur

    fprintf(stderr,"The last coup is in %d %d !\n", last[0], last[1]);
    if(sm.win == black || sm.win == white){
        return -1;
    }
    //======================= Determiner the morpion =======================

    if(last[1] <= 0 || last[1] > 9){
        fprintf(stderr,"The morpion %d doesn't exist !\n", last[1]);
        fprintf(stderr,"Please choose a new morpion(1-9)");
        prochain[0] = sasie2();
    }
    if(sm.morp[pos(last[1])].win != infini){
        fprintf(stderr,"The morpion %d is won by %c !\n", last[1], sm.morp[pos(last[1])].win);
        fprintf(stderr,"Please choose a new morpion(1-9)");
        prochain[0] = sasie2();    
    }else{
        prochain[0] = last[1];
    }    

    if(prochain[0] < 1 || prochain[0] > 9){
        return -2;
    }

    morpion mp = sm.morp[prochain[0] - 1];
    if(mp.win != infini){
        return -3;
    }

    //=======================Determine the position=======================

    fprintf(stderr,"You are %c in morpion %d !\n", sm.trait, prochain[0]);
    print_mp(mp);
    fprintf(stderr,"Please choose a new position(1-9)");
    prochain[1] = sasie2();

    if(prochain[1] < 1 || prochain[1] > 9){
        return -4;
    }
    if(mp.tableau[prochain[1] - 1] != infini){
        return -5;
    }
    return 0;
}
