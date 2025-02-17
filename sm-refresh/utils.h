
#pragma once
#include "morpion.h"
//int negamax();
extern int debug_mode;
int meilleur_coup(const supermopion, const int last[], int temps, int prochain[]);
int get_coup(const supermopion, const int last[], int temps, int prochain[]);
void str_to_sm(const char*, supermopion*, int last[]);
int sasie();
int sasie2();
int temps_to_depth(const int temps);
int get_new_morpion(const supermopion, const int last);
char N(const char trait);
int M(int A, int B);
int mcore(const morpion mp, const char trait);
int score(const supermopion);
int abs(int a);
int negamax0(const supermopion , const int m, const int depth, int alpha, int beta);
int negamax(const supermopion , const int m, const int depth, int alpha, int beta);
int minimax(const supermopion , const int m, const int depth);
int get_new_morpion(const supermopion , const int last);