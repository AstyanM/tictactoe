#pragma once
#include "morpion.h"
#define m1 0
#define m2 1
#define MAX 100
#define SIZE 9
#define MAXv(x, y) ((x) > (y) ? (x) : (y))

void str_to_morpion(const char* input, morpion* mp);
void print_tree(morpion mp, const char* file_path, int limite_depth);
int negamax(morpion mp, int depth, int alpha, int beta, FILE* file, int* node);
