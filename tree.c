#include "frequency.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Input {
    short bits;
    int frequency;
} Input;

typedef struct Node {
    short bits;
    int frequency;
    struct Node* left;
    struct Node* right;
    bool been;
    int first_code;
} Node;

typedef struct Output_tmp {
    short bits;
    int code;
    bool is_leaf;
} Output_tmp;

void make_tree( frequency_t *freqArray, struct Node *leafs, struct Node *nodes, int n, int *w){
    int d = n;
    for(int i = 0; i < n; i++){
        leafs[i].bits = freqArray[i].bits;
        leafs[i].frequency = freqArray[i].frequency;
    }
    for(int i = 0; i < n*2; i++){
        leafs[i].been = false;
        leafs[i].left = NULL;
        leafs[i].right = NULL;
    }
    int k = 0;
    for(int i = 0; i < n; i++){
        if(leafs[i].been == false){
            k++;
        }
    }
    int mini1, mini2;
    while(k>1){
        mini1 = -200;
        mini2 = -200;
        for(int i = 0; i < n; i++){
            if(leafs[i].been == false){
                if(mini1 == -200)
                    mini1 = i;
                else if(mini2 == -200)
                    mini2 = i;
                else if(leafs[i].frequency < leafs[mini1].frequency || leafs[i].frequency < leafs[mini2].frequency){
                    if(leafs[mini1].frequency < leafs[mini2].frequency){
                        mini2=i;
                    }
                    else{
                        mini1=i;
                    }
                }
            }
        }
        if(leafs[mini1].frequency > leafs[mini2].frequency){
            int temp = mini1;
            mini1 = mini2;
            mini2 = temp;
        }
        if(leafs[mini1].frequency == leafs[mini2].frequency && mini1 > mini2){
            int temp = mini1;
            mini1 = mini2;
            mini2 = temp;
        }

        leafs[n].frequency = leafs[mini1].frequency + leafs[mini2].frequency;
        leafs[n].left = &leafs[mini1];
        leafs[n].right = &leafs[mini2];
        leafs[mini1].been = true;
        leafs[mini2].been = true;
        n++;
        k = 0;
        for(int i = 0; i < n; i++){
            if(leafs[i].been == false){
                k++;
            }
        }
    }
    nodes = realloc(nodes, sizeof(Node)*(n));
    for(int i = 0; i < n; i++){
        nodes[i].bits = leafs[i].bits;
        nodes[i].frequency = leafs[i].frequency;
        nodes[i].left = leafs[i].left;
        nodes[i].right = leafs[i].right;
        nodes[i].been = leafs[i].been;
    }
    *w = n;
}

void dfs(struct Node *root, int tmp_code, struct Output_tmp *codes, int *ile){
    codes[*(ile)].code = tmp_code;
    if(root->left == NULL && root->right == NULL){
        codes[*(ile)].bits = root->bits;
    }
    else{
        codes[*(ile)].is_leaf = true;
    }
    (*ile)++;
    // printf("obecny kod: %d obecny bits: %d\n", tmp_code, root->bits);
    if(root->right != NULL){
        dfs(root->right, tmp_code*2, codes, ile);
    }
    if(root->left != NULL){
        dfs(root->left, tmp_code*2+1, codes, ile);
    }

}

void only_leaves(struct Output_tmp *codes_first, struct Output_tmp *codes_second, int k){
    int j = 0;
    for(int i = 0; i < k; i++){
        if(codes_first[i].is_leaf == false){
            codes_second[j].bits = codes_first[i].bits;
            codes_second[j].code = codes_first[i].code;
            j++;
        }
    }
}

void code_creator(struct Output_tmp *codes_second, struct Output *codes, int n) {
    for(int i = 0; i < n; i++) {
        int tmp = codes_second[i].code;
        int length = 0;
        while(tmp != 0) {
            tmp = tmp/2;
            length++;
        }
        length--;
        codes[i].bits = codes_second[i].bits;

        char* new_str = malloc(length + 1);
        tmp = codes_second[i].code;
        for(int j = length-1; j >= 0; j--) {
            if(tmp % 2 == 0) {
                new_str[j] = '0';
            } else {
                new_str[j] = '1';
            }
            tmp = tmp / 2;
        }
        new_str[length] = '\0';

        codes[i].code = new_str;
    }
}


Output * get_codes( frequency_t  *freqArray, int n) {

    struct Output *codes = malloc(sizeof(Output) * n);
    if(n > 1){
        struct Node *leafs = malloc(sizeof(Node) * (n * 2));
        struct Node *nodes = malloc(sizeof(Node) * (n * 2));
        int k = n;

        make_tree(freqArray, leafs, nodes, n, &k);

        struct Output_tmp *codes_first = malloc(sizeof(Output_tmp) * k);
        for(int i = 0; i < n; i++){
            codes_first[i].is_leaf = false;
        }
        Node root = nodes[k - 1];
        int lvl = 0;
        int ile = 0;

        dfs(&root, 1, codes_first, &ile);

        struct Output_tmp *codes_second = malloc(sizeof(Output_tmp) * n);

        only_leaves(codes_first, codes_second, k);

        code_creator(codes_second, codes, n);
    }
    else{
        codes[0].bits = freqArray[0].bits;
        codes[0].code = "0";
    }


    return codes;
}