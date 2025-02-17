#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "morpion.h"
int fine(int n){ // Fonction pratique
    if(n >= 0 && n < SIZE){
        return 1;
    }
    return 0;
}
char swin(supermopion sm){

    // BUT : Fonction de victoire du supermorpion

    morpion temp;
    for(int i=0; i<SIZE; i++){
        temp.tableau[i] = sm.morp[i].win;
    }
    sm.win = win(&temp);
    return sm.win;
}
char win(morpion* morp){

    // BUT : Fonction de victoire du morpion

    if(morp->win != infini){
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
    // int b = 0;
    // int w = 0;
    // int n = 0;
    // for(int i=0;i < SIZE; i++){
    //     if(morp->tableau[i] == white){
    //         w++;
    //     }else if(morp->tableau[i] == black){
    //         b++;
    //     }else n++;
    // }
    // if(!n){
    //     if(b > w) return black;
    //     else return white;
    // }
    morp->win = infini;
    return infini;
}

char* chaine(const supermopion sm){

    // BUT : Transformer un supermorpion en chaine FEN, non utilisée

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

    // BUT : Print le supermorpion, non utilisée

    printf("-------------\n");
    for(int i=0; i<SIZE; i+=3){
        for(int j=0; j<SIZE; j+=3){
            printf(" %c%c%c %c%c%c %c%c%c \n", sm.morp[i].tableau[j], sm.morp[i].tableau[j+1], sm.morp[i].tableau[j+2],
                   sm.morp[i+1].tableau[j], sm.morp[i+1].tableau[j+1], sm.morp[i+1].tableau[j+2],
                   sm.morp[i+2].tableau[j], sm.morp[i+2].tableau[j+1], sm.morp[i+2].tableau[j+2]);
        }
        printf("\n");
    }
    printf("-------------\n");
}
void print_mp(const morpion mp){

    // BUT : Print le morpion

    printf("-----\n");
    for(int i=0; i<SIZE; i+=3){
        printf(" %c%c%c \n", mp.tableau[i], mp.tableau[i+1], mp.tableau[i+2]);
    }
    printf("-----\n");
}

void initialize(supermopion* sm){

    // BUT : Créer un supermorpion vide, non utilisé

    for(int i=0; i<SIZE; i++){
        init_mp(sm->morp + i);
    }
}
void init_mp(morpion* mp){

    // BUT : Créer un morpion vide

    for(int i=0;i<SIZE;i++){
        mp->tableau[i] = infini;
    }
    mp->win = infini;
}

void make_dot(const char* const str) {

    // BUT : Créer le fichier DOT, ici, on utilise des printf au lieu des fprintf car le but est de marquer dans le dot

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
    printf("digraph  {\n");
    printf("\ta0 [shape=none label=<<TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"black\">\n");
    const char* c = str;
    for(int i = 0; i < SIZE && *c != '\0';) {
        if (*c == 'O') {
            if (i % 3 == 0) {
                printf("\t<TR>\n");
            }
            printf("\t<TD bgcolor=\"black\">\n");
            printf("\t\t<TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"white\">\n");
            for (int k = 0; k < 3; k++) {
                printf("\t\t\t<TR>\n");
                printf("\t\t\t<TD bgcolor=\"white\">&nbsp;</TD>\n");
                printf("\t\t\t<TD bgcolor=\"white\">&nbsp;</TD>\n");
                printf("\t\t\t<TD bgcolor=\"white\">&nbsp;</TD>\n");
                printf("\t\t\t</TR>\n");
            }

            printf("\t\t</TABLE>\n\t</TD>\n\n");
            i++;
            if (i % 3 == 0) {
                printf("\t</TR>\n");
            }
            c++;
        }       
        else if (*c == 'X') {
            if (i % 3 == 0) {
                printf("\t<TR>\n");
            }

            printf("\t<TD bgcolor=\"white\">\n");
            printf("\t\t<TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"black\">\n");
            for (int k = 0; k < 3; k++) {
                printf("\t\t\t<TR>\n");
                printf("\t\t\t<TD bgcolor=\"black\">&nbsp;</TD>\n");
                printf("\t\t\t<TD bgcolor=\"black\">&nbsp;</TD>\n");
                printf("\t\t\t<TD bgcolor=\"black\">&nbsp;</TD>\n");
                printf("\t\t\t</TR>\n");
            }

            printf("\t\t</TABLE>\n\t</TD>\n\n");
            i++;
            if (i % 3 == 0) {
                printf("\t</TR>\n");
            }
            c++;
        }
        else {
            if (i % 3 == 0) {
                printf("\t<TR>\n");
            }

            printf("\t<TD bgcolor=\"white\">\n");
            printf("\t\t<TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"black\">\n");

            while (j < 9) {
                if (*c <= '9' && *c > '0') {
                    for (int m = 0; m < *c - '0'; m++) {
                        if (j % 3 == 0) printf("\t\t\t<TR>\n");
                        printf("\t\t\t<TD bgcolor=\"white\">.</TD>\n");
                        j++;
                        if (j % 3 == 0) printf("\t\t\t</TR>\n");
                    }
                    c++;
                }
                else if (*c == 'x' || *c == 'o') {
                    if (j % 3 == 0) printf("\t\t\t<TR>\n");
                    printf("\t\t\t<TD bgcolor=\"white\">%c</TD>\n", *c);
                    j++;
                    if (j % 3 == 0) printf("\t\t\t</TR>\n");
                    c++;
                }
                else {
                    printf("%c", *c);
                    perror("in str, it's not pertinent to exist !\n");
                    return;
                }
            }            
            j = 0;
            printf("\t\t</TABLE>\n\t</TD>\n\n");
            i++;
            if (i % 3 == 0) {
                printf("\t</TR>\n");
            }
        } 
    }

    printf("\t</TABLE>\n");
    printf("\t>];\n");
    printf("}\n");
    // ... (continue printing the rest of the content)

    // Close the file
    fclose(file);

    return;
}
