#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "morpion.h"

int fine(int n){ // Fonction pratique qui teste si n est dans la bonne plage de valeur 
    if(n >= 0 && n < SIZE){
        return 1;
    }
    return 0;
}

int pos(const int a){ // Fonction pratique qui décale d'1
    return a-1;
}
void coup(morpion* mp, const int i, const char trait){

    // BUT : ajouter un coup dans un morpion 

    // i: 1-9
    if(i<1 || i > 9){
        perror("i is out of range !\n");
        return;
    }
    if(mp->tableau[i - 1] != infini){
        printf("The position %d has been occupied by %c!\n", i, mp->tableau[i - 1]);
        return;
    }
    mp->tableau[i - 1] = trait;
    win(mp);

    
}

void scoup(supermopion* sm, const int m, const int i){
    
    // BUT : Ajouter un coup dans un supermorpion

    // m : 1-9, i : 1-9
    if(m<1 || m > 9){
        perror("m is out of range !\n");
        return;
    }
    if(sm->morp[pos(m)].win != infini){
        /*printf("The morpion %d has been remported by %c !\n", m, sm->morp[pos(m)].win);*/
        return;
    }
    coup(sm->morp + m - 1, i, sm->trait);
    sm->trait = sm->trait == black ? white:black;
    swin(sm);

    
}

morpion get_large(const supermopion sm){

    // BUT : Transforme un supermorpion en morpion classique (les morpions en cours sont transformés en infinis)

    morpion large;
    init_mp(&large);

    for(int i=0; i<SIZE; i++){
        large.tableau[i] = sm.morp[i].win;
    }
    large.win = infini;
    win(&large);
    return large;

}

char swin(supermopion* sm){

    // BUT : Determine la victoire d'un supermorpion

    /*morpion temp;*/
    /*for(int i=0; i<SIZE; i++){*/
    /*temp.tableau[i] = sm->morp[i].win;*/
    /*}*/
    if(sm->win == black || sm->win == white){
        return sm->win;
    }
    sm->win = get_large(*sm).win;
    /*printf("Winner is %c,  %c!\n", sm->win, get_large(*sm).win);*/
    return sm->win;
}
char win(morpion* morp){

    // BUT : Determine la victoire d'un morpion

    if(morp->win == black || morp->win == white){
        return morp->win;
    }
    int t[][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };
    for(int i=0; i< 8; i++){
        if(morp->tableau[t[i][0]] == morp->tableau[t[i][1]] && morp->tableau[t[i][1]] == morp->tableau[t[i][2]] && morp->tableau[t[i][1]] != infini){
            morp->win = morp->tableau[t[i][0]];
            return morp->win;   
        }
    }
    int b = 0;
    int w = 0;
    int n = 0;
    for(int i=0;i < SIZE; i++){
        if(morp->tableau[i] == white){
            w++;
        }else if(morp->tableau[i] == black){
            b++;
        }else n++;
    }
    if(!n)  morp->win = b > w ? black : white;
    else    morp->win = infini;
    return infini;
}

char* chaine(const supermopion sm){

    // BUT : Créer une chaîne de caractère à partir d'un supermorpion

    static char str[100] = "";
    if(strcmp(str, "") != 0) strcpy(str, "");
    for(int i=0;i<SIZE;i++){
        if(sm.morp[i].win == black){
            sprintf(str + strlen(str), "%c", 'X');
            str[strlen(str) + 1] = '\0';
        }
        else if(sm.morp[i].win == white){
            sprintf(str + strlen(str), "%c", 'O');
            str[strlen(str) + 1] = '\0';
        }
        else if(sm.morp[i].win == infini){
            int t = 0;
            for(int m=0;m<9;m++){
                if(sm.morp[i].tableau[m] == infini){
                    t++;
                }else{
                    if(t != 0){
                        sprintf(str + strlen(str), "%d", t);
                        str[strlen(str) + 1] = '\0';
                        t = 0;
                    }
                    if(sm.morp[i].tableau[m] == black){
                        str[strlen(str)] = black;
                        str[strlen(str) + 1] = '\0';
                    }
                    else if(sm.morp[i].tableau[m] == white){
                        str[strlen(str)] = white;
                        str[strlen(str) + 1] = '\0';
                    }
                }
                if(t != 0 && m == 8){
                    sprintf(str + strlen(str), "%d", t);
                    str[strlen(str) + 1] = '\0';
                    t = 0;
                }
            }
        }
    }
    return str;
}

void print_sm(const supermopion sm){

    // BUT : Affichier un supermorpion

    printf("-----------------\n");
    for(int i=0; i<SIZE; i+=3){
        printf(" %d    %d    %d\n", i+1, i+2, i+3);
        for(int j=0; j<SIZE; j+=3){
            printf("  %c%c%c  %c%c%c  %c%c%c  \n", sm.morp[i].tableau[j], sm.morp[i].tableau[j+1], sm.morp[i].tableau[j+2],
                   sm.morp[i+1].tableau[j], sm.morp[i+1].tableau[j+1], sm.morp[i+1].tableau[j+2],
                   sm.morp[i+2].tableau[j], sm.morp[i+2].tableau[j+1], sm.morp[i+2].tableau[j+2]);
        }
    }
    printf("\n-----------------\n");

}
void print_mp(const morpion mp){

    // BUT : Affichier un morpion

    printf("----------\n");
    for(int i=0; i<SIZE; i+=3){
        printf(" %d %c %c %c \n",(9-i)/3, mp.tableau[i], mp.tableau[i+1], mp.tableau[i+2]);
    }
    printf("   a b c\n");
    printf("----------\n");
}

void initialize(supermopion* sm){

    // BUT : Créer un supermorpion

    for(int i=0; i<SIZE; i++){
        init_mp(sm->morp + i);
    }
}
void init_mp(morpion* mp){

    // BUT : initialiser un morpion

    for(int i=0;i<SIZE;i++){
        mp->tableau[i] = infini;
    }
    mp->win = infini;
}

void make_dot(const char* const str) {

    // BUT : Créer le dot 

    // Open the file for writing
    char* filename = "g.dot";
    FILE *file = fopen(filename, "w");

    int j = 0;
    // Check if the file is opened successfully
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return;
    }

    // Print the content to the file
    fprintf(file, "digraph  {\n");
    fprintf(file, "\ta0 [shape=none label=<<TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"black\">\n");
    const char* c = str;
    for(int i = 0; i < SIZE && *c != '\0';) {
        if (*c == 'O') {
            if (i % 3 == 0) {
                fprintf(file, "\t<TR>\n");
            }
            fprintf(file, "\t<TD bgcolor=\"black\">\n");
            fprintf(file, "\t\t<TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"white\">\n");
            for (int k = 0; k < 3; k++) {
                fprintf(file, "\t\t\t<TR>\n");
                fprintf(file, "\t\t\t<TD bgcolor=\"white\">&nbsp;</TD>\n");
                fprintf(file, "\t\t\t<TD bgcolor=\"white\">&nbsp;</TD>\n");
                fprintf(file, "\t\t\t<TD bgcolor=\"white\">&nbsp;</TD>\n");
                fprintf(file, "\t\t\t</TR>\n");
            }

            fprintf(file, "\t\t</TABLE>\n\t</TD>\n\n");
            i++;
            if (i % 3 == 0) {
                fprintf(file, "\t</TR>\n");
            }
            c++;
        }       
        else if (*c == 'X') {
            if (i % 3 == 0) {
                fprintf(file, "\t<TR>\n");
            }

            fprintf(file, "\t<TD bgcolor=\"white\">\n");
            fprintf(file, "\t\t<TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"black\">\n");
            for (int k = 0; k < 3; k++) {
                fprintf(file, "\t\t\t<TR>\n");
                fprintf(file, "\t\t\t<TD bgcolor=\"black\">&nbsp;</TD>\n");
                fprintf(file, "\t\t\t<TD bgcolor=\"black\">&nbsp;</TD>\n");
                fprintf(file, "\t\t\t<TD bgcolor=\"black\">&nbsp;</TD>\n");
                fprintf(file, "\t\t\t</TR>\n");
            }

            fprintf(file, "\t\t</TABLE>\n\t</TD>\n\n");
            i++;
            if (i % 3 == 0) {
                fprintf(file, "\t</TR>\n");
            }
            c++;
        }
        else {
            if (i % 3 == 0) {
                fprintf(file, "\t<TR>\n");
            }

            fprintf(file, "\t<TD bgcolor=\"white\">\n");
            fprintf(file, "\t\t<TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"black\">\n");

            while (j < 9) {
                if (*c <= '9' && *c > '0') {
                    for (int m = 0; m < *c - '0'; m++) {
                        if (j % 3 == 0) fprintf(file, "\t\t\t<TR>\n");
                        fprintf(file, "\t\t\t<TD bgcolor=\"white\">.</TD>\n");
                        j++;
                        if (j % 3 == 0) fprintf(file, "\t\t\t</TR>\n");
                    }
                    c++;
                }
                else if (*c == 'x' || *c == 'o') {
                    if (j % 3 == 0) fprintf(file, "\t\t\t<TR>\n");
                    fprintf(file, "\t\t\t<TD bgcolor=\"white\">%c</TD>\n", *c);
                    j++;
                    if (j % 3 == 0) fprintf(file, "\t\t\t</TR>\n");
                    c++;
                }
                else {
                    printf("%c", *c);
                    perror("in str, it's not pertinent to exist !\n");
                    return;
                }
            }            
            j = 0;
            fprintf(file, "\t\t</TABLE>\n\t</TD>\n\n");
            i++;
            if (i % 3 == 0) {
                fprintf(file, "\t</TR>\n");
            }
        } 
    }

    fprintf(file, "\t</TABLE>\n");
    fprintf(file, "\t>];\n");
    fprintf(file, "}\n");
    // ... (continue printing the rest of the content)

    // Close the file
    fclose(file);

    char* path = ".";

    if (getenv("SMPATH") != NULL) {
        path = getenv("SMPATH");
    }
    
    

    char new_path[400];
    sprintf(new_path, "%s/g.png", path);

    char command[100];
    sprintf(command, "dot -Tpng -o %s %s",new_path, filename);
    system(command);

    return;
}
