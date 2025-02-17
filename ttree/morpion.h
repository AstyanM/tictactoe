#pragma once
#define SIZE 9
#define black 'x'
#define white 'o'
#define infini '.'
typedef struct{
    char tableau[SIZE];
    char win;
    char trait;
}morpion;
typedef struct{
    morpion morp[SIZE];
    char win;
}supermopion;

char swin(supermopion);
char win(morpion*);
char* chaine(supermopion sm);
void print_sm(const supermopion);
void print_mp(const morpion);
void initialize(supermopion*);
void init_mp(morpion*);
void make_dot(const char* const);
