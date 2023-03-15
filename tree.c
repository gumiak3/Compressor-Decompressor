#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Input {
    int symbol;
    int frequency; 
} Input;

typedef struct Node {
    int symbol;
    int frequency;
    struct Node* left;
    struct Node* right;
} Node;

void make_tree(struct Input *tab, int n, Node* root){
    Node* nodes = malloc(sizeof(Node)*n);
    for(int i = 0; i < n; i++){
        nodes[i].symbol = tab[i].symbol;
        nodes[i].frequency = tab[i].frequency;
        nodes[i].left = NULL;
        nodes[i].right = NULL;
    }
    while(n > 1){
        Node* parent = malloc(sizeof(Node));
        parent->frequency = nodes[0].frequency + nodes[1].frequency;
        parent->left = &nodes[0];
        parent->right = &nodes[1];
        for(int i = 2; i < n; i++){
            nodes[i-2] = nodes[i];
        }
        nodes[n-2] = *parent;
        n--;
        for(int i = 0; i < n-1; i++){
            for(int j = 0; j < n-i-1; j++){
                if(nodes[j].frequency > nodes[j+1].frequency){
                    Node temp = nodes[j];
                    nodes[j] = nodes[j+1];
                    nodes[j+1] = temp;
                }
            }
        }
    }
    root = &nodes[0];
    //printf("%d", root->frequency);
}

void dfs(Node* root, char** codes, char* code, int depth){
    if(root->left == NULL && root->right == NULL){
        strcpy(codes[root->symbol], code);
        printf("%s \n", codes[root->symbol]);
        return;
    }
    code[depth] = '0';
    dfs(root->left, codes, code, depth+1);
    code[depth] = '1';
    dfs(root->right, codes, code, depth+1);
}


int main(void){
    int n = 7;
    struct Input *tab = malloc(sizeof(struct Input)*n);
    for(int i = 0; i < n; i++){
        tab[i].frequency = 2*i;
        tab[i].symbol = i+1;
    }
    char** codes = malloc(sizeof(char*)*(1<<16));
    for(int i = 0; i < (1<<16); i++){
        codes[i] = malloc(sizeof(char)*16);
    }
    Node* root = malloc(sizeof(struct Node));
    char* tmp = malloc(sizeof(char)*16);


    make_tree(tab, n, root);
    dfs(root, codes, tmp, 0);

    printf("%s", codes[6]);
    // codes[0] = "010101010";
    
    //printf("%s", codes[0]);
    //printf("%d %d", tab[0].symbol, tab[0].frequency);
}
