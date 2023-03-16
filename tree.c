#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Input {
    short symbol;
    int frequency; 
} Input;

typedef struct Node {
    short symbol;
    int frequency;
    struct Node* left;
    struct Node* right;
    bool bylo;
} Node;

void make_tree(struct Input *tab, int n, Node* root){
    Node* nodes = malloc(sizeof(Node)*n);
    for(int i = 0; i < n; i++){
        nodes[i].symbol = tab[i].symbol;
        nodes[i].frequency = tab[i].frequency;
        nodes[i].left = NULL;
        nodes[i].right = NULL;
    }

    for(int i = 0; i < 2*n; i++){
        nodes[i].bylo = false;
    }

    int k = 2, mini1, mini2;

    while(k>1){
        k=0;
        for(int i = 0; i < n; i++){
            if(nodes[i].bylo == false){
                k++;
            }
        }
        printf("diho raz ");
        printf("%d ", k);
        mini1 = -1;
        mini2 = -1;
        for(int i = 0; i < n; i++){
            if(nodes[i].bylo == false){
                if(mini1 == -1)
                    mini1 = i;
                else if(mini2 == -1)
                    mini2 = i;
                else if(nodes[i].frequency < nodes[mini1].frequency || nodes[i].frequency < nodes[mini2].frequency){
                    if(nodes[mini1].frequency < nodes[mini2].frequency){
                        mini2=i;
                    }
                    else{
                        mini1=i;
                    }
                }
                
            }
        }
        printf("%d %d ", mini1, mini2);

        nodes[n].frequency = nodes[mini1].frequency + nodes[mini2].frequency;
        nodes[n].left = &nodes[mini1];
        nodes[n].right = &nodes[mini2];
        printf("%d %d %d \n", nodes[n].left->symbol, nodes[n].right->symbol, nodes[n].frequency);
        nodes[mini1].bylo = true;
        nodes[mini2].bylo = true;
        n++;
    }

    // if(nodes)
    // printf("%d", parent->left->symbol);
    // nodes[n-2] = *parent;
    // n--;

    // for(int i = 0; i < n-1; i++){
    //     for(int j = 0; j < n-i-1; j++){
    //         if(nodes[j].frequency > nodes[j+1].frequency){
    //             Node temp = nodes[j];
    //             nodes[j] = nodes[j+1];
    //             nodes[j+1] = temp;
    //         }
    //     }
    // }


    // for(int i = 0; i < n; i++){
    //     printf("%d %d \n", nodes[i].symbol, nodes[i].frequency);
    // }
    // root = &nodes[0];
    // printf("%d \n", root->frequency);
}

int main(void){
    int n;
    n = 6;
    struct Input *tab = malloc(sizeof(struct Input)*n);
    tab[0].symbol = 1011;
    tab[0].frequency = 1;
    tab[1].symbol = 1010;
    tab[1].frequency = 2;
    tab[2].symbol = 1001;
    tab[2].frequency = 2;
    tab[3].symbol = 1100;
    tab[3].frequency = 4;
    tab[4].symbol = 1111;
    tab[4].frequency = 4;
    tab[5].symbol = 1110;
    tab[5].frequency = 18;
    struct Node* leafs = malloc(sizeof(Node)*n*2);

    // for(int i = 0; i < n; i++){
    //     printf("%d %d \n", leafs[i].symbol, leafs[i].frequency);
    // }
    struct Node* root = malloc(sizeof(Node));
    make_tree(tab, n, root);
    // printf("%hd", root->right->symbol);
}
