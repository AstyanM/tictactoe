#include <stdio.h>
#include "string.h"
/*#include "morpion.h"*/
#include "utils.h"

int is_fini(morpion mp);
int check_line(const morpion *mp, int line[], char color);
void print_label(FILE *file, int score, int node);
int calcul_score(morpion mp);
int print_node(FILE *file, morpion mp);
int minimax(morpion mp, int depth, FILE *file,int *node_root);

void str_to_morpion(const char *input, morpion *mp)
{
    int i = 0;
    int index = 0;

    while (input[i] != '\0')
    {
        if (input[i] == 'o' || input[i] == 'x')
        {
            mp->tableau[index++] = input[i];
        }
        else if (input[i] >= '1' && input[i] <= '9')
        {
            int k = input[i] - '0';
            while (k-- > 0 && index < SIZE)
            {
                mp->tableau[index++] = '.';
            }
        }
        else if (input[i] == ' ')
        {
            // Read the next character as the value of mp->trait
            i++;
            if (input[i] == 'o' || input[i] == 'x')
            {
                mp->trait = input[i];
            }
        }
        i++;
    }

    // Ensure the tableau is filled with '.' if needed
    while (index < SIZE)
    {
        mp->tableau[index++] = '.';
    }
}

void print_tree(morpion mp, const char *file_path, int limite_depth)
{
    FILE *file = fopen(file_path, "w");

    // Check if the file was opened successfully
    if (file == NULL)
    {
        perror("Error opening file");
        return; // Return an error code
    }

    // Close the file

    printf("digraph{\n");
    int node = 0;
    minimax(mp, limite_depth, file, &node);

    printf("}\n");
    fclose(file);
}

int minimax(morpion mp, int depth, FILE *file,int *node_root){

    *node_root = print_node(file, mp); // the name of itself
    if (depth == 0 || is_fini(mp))
    {
        // print the leaf
        int val = calcul_score(mp);
        // *node_root = print_node(file, mp);
        print_label(file, val, *node_root);
        return val;
    }

    /*SI ( traitÀJ ) // trait à J:MAX*/
    int val;
    morpion origin = mp;
    int ret = 0;
    int node = 0;
    if(mp.trait == black){
        val = -500;
        /*POUR chaque enfant e de node*/
        for(int i=1;i<=SIZE;i++){
            if(mp.tableau[i-1]== infini){
                mp.tableau[i-1] = black;
                mp.trait = (mp.trait == black ? white : black);
                ret = minimax(mp, depth-1, file, &node);
                val = (val > ret)? val: ret;
                printf("m_%d-> m_%d [label=\" %d, %d\"];\n", *node_root, node, (i-1) / 3, (i-1) % 3);
            }
            mp = origin;
        }
    }
    /*SINON // trait à l’opposant:MIN*/
    else{
        val = 500;
        /*POUR chaque enfant e de node*/
        for(int i=1;i<=SIZE;i++){
            if(mp.tableau[i-1]== infini){
                mp.tableau[i-1] = white;
                mp.trait = (mp.trait == black ? white : black);
                ret = minimax(mp, depth-1, file, &node);
                val = (val < ret)? val: ret;
                printf("m_%d-> m_%d [label=\" %d, %d\"];\n", *node_root, node, (i-1) / 3, (i-1) % 3);
            }
            mp = origin;
        }
    }
    
    
    /*for(int i=1;i<=SIZE;i++){*/
    /*if(mp->tableau[i - 1]==infini){*/
    /*return i;*/
    /*}*/
    /*}*/
    print_label(file, val, *node_root);
    return val;
}

int negamax(morpion mp, int depth, int alpha, int beta, FILE *file, int *node_root)
{
    // recursive
    /*negamax(mp, m, depth, alpha, beta, file);*/
    *node_root = print_node(file, mp); // the name of itself
    if (depth == 0 || is_fini(mp))
    {
        // print the leaf
        int val = calcul_score(mp);
        // *node_root = print_node(file, mp);
        print_label(file, val, *node_root);
        return val;
    }
    int val = -MAX;
    morpion mp_origin = mp;
    /*printf("node_root before:%d\n", *node_root);*/
    /*printf("print_node: %d\n", print_node(file, mp));*/
    /*val = MAXv(val, −negamax(mp, depth − 1, -beta, -alpha, file));*/
    /*printf("node_root after:%d\n", *node_root);*/

    /*printf("node root:%d\n", *node_root);*/
    int node = 0;
    for (int i = 0; i < SIZE; i++)
    {
        /*POUR chaque enfant e de node*/
        if (mp.tableau[i] != infini)
        {
            continue;
        }
        mp.tableau[i] = (mp.trait == m1 ? white : black);
        mp.trait = (mp.trait == black ? white : black);
        // print the content of the node into the file
        /*printf("node before:%d\n", node);*/
        int limite = -negamax(mp, depth - 1, -beta, -alpha, file, &node);
        val = MAXv(val, limite);
        /*node ++;*/
        /*printf("node after:%d\n", node);*/
        // TODO: print the connection
        printf("m_%d-> m_%d [label=\" %d, %d\"];\n", *node_root, node, i / 3, i % 3);
        if (val >= beta)
            break;
        alpha = MAXv(alpha, val); // maj alpha
        mp = mp_origin;
    }
    print_label(file, val, *node_root);
    return val;
}

void print_label(FILE *file, int score, int node)
{
    printf("m_%d[xlabel=\"%d\"]\n", node, score);
}
int print_node(FILE *file, morpion mp)
{
    static int node = 0;
    printf("m_%d [shape=none label=<<TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"black\">\n", node);

    for (int i = 0; i < 3; ++i)
    {
        printf("\t<TR>\n");
        for (int j = 0; j < 3; ++j)
        {
            printf("\t\t<TD bgcolor=\"white\">%c</TD>\n", mp.tableau[3 * i + j]);
        }
        printf("\t</TR>\n");
    }
    char color[6];
    if (mp.trait == white)
    {
        strcpy(color, "red");
    }
    else
    {
        strcpy(color, "green");
    }
    if (node == 0)
        printf("\t<TR><TD bgcolor=\"%s\" colspan=\"3\">m_init</TD></TR>\n", color);
    else
        printf("\t<TR><TD bgcolor=\"%s\" colspan=\"3\">m_%d</TD></TR>\n", color, node - 1);
    /*fprintf(file, "\t<TR><TD bgcolor=\"%s\" colspan=\"3\">m_%d</TD></TR>\n",color, node);*/

    printf("</TABLE>>];\n");

    /*printf("node in print_node: %d\n", node);*/
    return node++;
}

int calcul_score(morpion mp)
{
    if (win(&mp) == white)
    {
        return 1;
    }
    if (win(&mp) == black)
    {
        return -1;
        // return -9;
    }
    return 0;
    // int t[][3] = {
    //     {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
    //     {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
    //     {0, 4, 8}, {2, 4, 6}
    // };
    // int pos = 0, neg = 0;
    // char color = (mp.trait == 0 ? white:black);
    // for (int i = 0; i < 8; ++i) {
    //     if (check_line(&mp, t[i], color)) {
    //         pos++;
    //     } else if (check_line(&mp, t[i], !color)){
    //         neg++;
    //     }
    // }
    // return pos - neg;
}

// int check_line(const morpion* mp, int line[], char color) {
//     return ((mp->tableau[line[0]] == color || mp->tableau[line[0]] == infini) && (mp->tableau[line[1]] == color || mp->tableau[line[1]] == infini) && (mp->tableau[line[2]] == color || mp->tableau[line[2]] == infini));
// }
int is_fini(morpion mp)
{
    if (win(&mp) == white || win(&mp) == black)
    {
        return 1;
    }
    for (int i = 0; i < SIZE; i++)
    {
        if (mp.tableau[i] == '.')
        {
            return 0;
        }
    }
    return 1;
}
