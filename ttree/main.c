#include <stdio.h>
#include "utils.h"
#include "morpion.h"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_string>\n", argv[0]);
        return 1;
    }

    const char* input = argv[1];
    morpion mp; // Initialize the Morpion structure
    init_mp(&mp);

    str_to_morpion(input, &mp);
    print_tree(mp, "g1.dot", 9);

    // Print the resulting Morpion structure
    //printf("tableau: %s\n", mp.tableau);
    //printf("trait: %c\n", mp.trait);

    return 0;
}
