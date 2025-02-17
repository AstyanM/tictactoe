

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
    char trait;
}supermopion;



extern int debug_mode;
char swin(supermopion*);
char win(morpion*);

morpion get_large(const supermopion sm);

int pos(const int a);
void coup(morpion* mp, const int i, const char trait);
void scoup(supermopion* sm, const int m, const int i);
char* chaine(supermopion sm);
void print_sm(const supermopion);
void print_mp(const morpion);
void initialize(supermopion*);
void init_mp(morpion*);
void make_dot(const char* const);

