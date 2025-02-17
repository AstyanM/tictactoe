#ifndef UTILS_H
#define UTILS_H


#pragma once
#include "morpion.h"
//int negamax();

int meilleur_coup(const supermopion, const int last[], int temps, int prochain[]);
int get_coup(const supermopion, const int last[], int temps, int prochain[]);
void str_to_sm(const char*, supermopion*, int last[]);
int sasie();
int sasie2();
void temps_to_depth(const double temps);
int is_board_full(const morpion);


#endif
