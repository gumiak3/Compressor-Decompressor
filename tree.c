#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct Input {
    short symbol;
    int frequency;
} Input;

typedef struct Node {
    short symbol;
    int frequency;
    struct Node* left;
    struct Node* right;
    bool been;
    int first_code;
} Node;

typedef struct Output_tmp {
    short symbol;
    int code;
} Output_tmp;

typedef struct Output {
    short symbol;
    char *code;
    int code_length;
} Output;

void make_tree(struct Input *tab, struct Node *leafs, struct Node *nodes, int n, int *w){
    int d = n;
    for(int i = 0; i < n; i++){
        leafs[i].symbol = tab[i].symbol;
        leafs[i].frequency = tab[i].frequency;
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
    // printf("%d \n", k);
    int mini1, mini2;
    while(k>1){
        mini1 = -1;
        mini2 = -1;
        for(int i = 0; i < n; i++){
            if(leafs[i].been == false){
                if(mini1 == -1)
                    mini1 = i;
                else if(mini2 == -1)
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
        // printf("%d %d\n", mini1, mini2);

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
        nodes[i].symbol = leafs[i].symbol;
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
        codes[*(ile)].symbol = root->symbol;
    }
    else{
        codes[*(ile)].symbol = -1;
    }
    (*ile)++;
    // printf("obecny kod: %d obecny symbol: %d\n", tmp_code, root->symbol);
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
        if(codes_first[i].symbol != -1){
            codes_second[j].symbol = codes_first[i].symbol;
            codes_second[j].code = codes_first[i].code;
            j++;
        }
    }
}

void code_creator(struct Output_tmp *codes_second, struct Output *codes, int n){
    for(int i = 0; i < n; i++){
        int tmp = codes_second[i].code;
        int length = 0;
        while(tmp != 0){
            tmp = tmp/2;
            length++;
        }
        length--;
        codes[i].code_length = length;
        codes[i].symbol = codes_second[i].symbol;
        printf("%d ", length);
        if(length > 1)
            codes[i].code = malloc(sizeof(char)*length);
        else
            codes[i].code = malloc(sizeof(char));
        tmp = codes_second[i].code;
        for(int j = length-1; j >= 0; j--){
            if(tmp%2 == 0)
                codes[i].code[j] = '0';
            else
                codes[i].code[j] = '1';
            // codes[i].code[j] = tmp%2;
            tmp = tmp/2;
        }
    }
}


int main(void){
    int n, k;
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
    tab[5].symbol = 1100;
    tab[5].frequency = 18;
    struct Node* leafs = malloc(sizeof(Node)*(n*2));
    struct Node* nodes = malloc(sizeof(Node)*(n*2));

    make_tree(tab, leafs, nodes, n, &k);

    struct Output_tmp* codes_first = malloc(sizeof(Output_tmp)*k);
    Node root = nodes[k-1];
    int lvl = 0;
    int ile = 0;

    dfs(&root, 1, codes_first, &ile);

    struct Output_tmp* codes_second = malloc(sizeof(Output_tmp)*n);

    only_leaves(codes_first, codes_second, k);

    struct Output* codes = malloc(sizeof(Output)*n);

    code_creator(codes_second, codes, n);

    // for(int i = 0; i < ile; i++){
    // 	printf("Symbol: %d Code: %d \n", codes_first[i].symbol, codes_first[i].code);
    // }

    // printf("%d \n", n);

    // printf("po usunieciu \n");
    // for(int i = 0; i < n; i++){
    // 	printf("Symbol: %d Code: %d \n", codes_second[i].symbol, codes_second[i].code);
    // }

    // printf("po stworzeniu kodow \n");

    // for(int i = 0; i < n; i++){
    // 	printf("Symbol: %d Code: %s o dlugosci kodu %d \n", codes[i].symbol, codes[i].code, codes[i].code_length);
    // }

    // for(int i = 0; i < k; i++){
    // 	printf("kod %d: %d\n", i, nodes[i].symbol);
    // }

    // for(int i = 0; i < n; i++){
    // 	printf("Symbol: %d Code: ", codes[i].symbol);
    // 	for(int j = 0; j < n; j++){
    // 		if(codes[i].code[j] == 2)
    // 			break;
    // 		printf("%d", codes[i].code[j]);
    // 	}
    // 	printf("\n");
    // }

    // for(int i = 0; i < k; i++){
    // 	printf("w wezle nr %d jest symbol %d \n", i, nodes[i].symbol);
    // }
    // printf("n = %d \n", n);
    // printf("k = %d \n", k);



    // for(int i = 0; i < k; i++){
    // 	printf("Node at index %d: \n", i);
    // 	printf("Symbol:%d ", nodes[i].symbol);
    // 	printf("frequency:%d ", nodes[i].frequency);
    // 	if(nodes[i].left != NULL){
    // 	    printf("Left child symbol: %d ", nodes[i].left->symbol);
    // 	}
    // 	else{
    // 	    printf("Left child is NULL ");
    // 	}
    // 	if(nodes[i].right != NULL){
    // 	    printf("Right child symbol: %d ", nodes[i].right->symbol);
    // 	}
    // 	else{
    // 	    printf("Right child is NULL ");
    // 	}
    // 	printf("Been:%d ", nodes[i].been);
    // 	printf("\n\n");
    // }
    return 0;
}

// obecny kod: 2 obecny symbol: 1110
// obecny kod: 12 obecny symbol: 1111
// obecny kod: 13 obecny symbol: 1100
// obecny kod: 15 obecny symbol: 1001
// obecny kod: 28 obecny symbol: 1010
// obecny kod: 29 obecny symbol: 1011