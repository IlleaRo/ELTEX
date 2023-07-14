#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "BRTree/tree.h"
#include "system/input.h"




int main() {
    TreeNode* tree = NULL;

    int answer;
    while (true){
        printf("Add element (1), print tree (2), delete tree (3), search node with value (4) or exit (5)?\n ---> ");
        while ((answer=readInt()) < 1 || answer > 5) printf("Try again!\n ---> ");
        system("clear");
        switch (answer) {
            case 1:{
                printf(" ---> ");
                insertRed(&tree,readInt());
                printf("\n");
                break;
            }
            case 2:{
                showTree(tree,0,0);
                printf("Press enter!");
                getchar();
                break;
            }
            case 3: {
                clearTree(&tree);
                break;
            }
            case 4: {
                printf(" ---> ");
                printf("Searched node: %p\n", find(tree, readInt()));
                getchar();
                break;
            }
            case 5: {
                clearTree(&tree);
                return 0;
            }
            default:
                printf("Unknown error!\n");
        }
        system("clear");
    }
}

