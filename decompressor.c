#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


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
} Output_tmp;

typedef struct Output {
    short bits;
    char *code;
} Output;

typedef struct frequency_t{
    short bits;
    int frequency;
}frequency_t;

typedef struct decode_t{
	short bits;
	int decimal_code;
	char *binary_code;
} decode_t;

typedef struct decode_t2{
	short bits;
	int decimal_code;
} decode_t2;

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
        codes[*(ile)].bits = -1;
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
        if(codes_first[i].bits != -1){
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
//        printf("obecny kod to: %s, a jego dlugosc to: %d\n", codes[i].code, strlen(codes[i].code));
    }
}


Output * get_codes( frequency_t  *freqArray, int n) {


    struct Node *leafs = malloc(sizeof(Node) * (n * 2));
    struct Node *nodes = malloc(sizeof(Node) * (n * 2));
    int k = n;

    make_tree(freqArray, leafs, nodes, n, &k);

    struct Output_tmp *codes_first = malloc(sizeof(Output_tmp) * k);
    Node root = nodes[k - 1];
    int lvl = 0;
    int ile = 0;

    dfs(&root, 1, codes_first, &ile);

    struct Output_tmp *codes_second = malloc(sizeof(Output_tmp) * n);

    only_leaves(codes_first, codes_second, k);

    struct Output *codes = malloc(sizeof(Output) * n);

    code_creator(codes_second, codes, n);


    return codes;
}

int decimal_code_maker(decode_t decimal_codes){
	int length = strlen(decimal_codes.binary_code);
	char *tmp = malloc(sizeof(char) * length);
	int tmp_code = 1;
	for(int i = 0; i < length; i++){
		if(decimal_codes.binary_code[i] == '1'){
			tmp_code = tmp_code * 2 + 1;
		}
		else{
			tmp_code = tmp_code * 2;
		}
	}
	printf("%d\n", tmp_code);
	return tmp_code;
}
int to_decimal(int n)
{
	int decimalNumber = 0, i = 0, remainder;
	while (n!=0)
	{
		remainder = n%10;
		n /= 10;
		decimalNumber += remainder*pow(2,i);
		++i;
	}
	return decimalNumber;
}


int main(void){
	int n = 4;
	char *wejscie = "01101111000000001";
	frequency_t *freqArray = malloc(sizeof(frequency_t) * n);
	freqArray[0].bits = 1010;
	freqArray[0].frequency = 1;
	freqArray[1].bits = 1111;
	freqArray[1].frequency = 2;
	freqArray[2].bits = 1001;
	freqArray[2].frequency = 2;
	freqArray[3].bits = 1011;
	freqArray[3].frequency = 4;

	Output *codes = get_codes(freqArray, n);

	decode_t *decimal_codes= malloc(sizeof(decode_t) * n);

	for(int i = 0; i < n; i++){
		decimal_codes[i].bits = codes[i].bits;
		decimal_codes[i].binary_code = codes[i].code;
		decimal_codes[i].decimal_code = decimal_code_maker(decimal_codes[i]);
	}
	for(int i = 0; i < n; i++){
		printf("bits: %d binary_code: %s decimal_code %d \n", decimal_codes[i].bits, decimal_codes[i].binary_code, decimal_codes[i].decimal_code);
	}

	decode_t2 *final_codes = malloc(sizeof(decode_t2) * pow(2, 8));
	for(int i = 0; i < pow(2, 8); i++){
		final_codes[i].bits = -1;
		final_codes[i].decimal_code = -1;
	}
	for(int i = 0; i < n; i++){
		final_codes[decimal_codes[i].decimal_code].bits = decimal_codes[i].bits;
		final_codes[decimal_codes[i].decimal_code].decimal_code = decimal_codes[i].decimal_code;
	}
	for(int i = 0; i < pow(2, 8); i++){
		if(final_codes[i].bits != -1){
			printf("bits: %d decimal_code %d \n", final_codes[i].bits, final_codes[i].decimal_code);
		}
	}
	int length = strlen(wejscie);
	int tmp = 1;
	for(int i = 0; i < length; i++){
			if(wejscie[i] == '1'){
				tmp = tmp * 2 + 1;
			}
			else{
				tmp = tmp * 2;
			}
		printf("%d ", tmp);
		if(final_codes[tmp].bits != -1){
			printf("%d \n", final_codes[tmp].bits);
			tmp = 1; 
		}
		
	}


	return 0;
}
// for(int i = 0; i < ile; i++){
// 	printf("bits: %d Code: %d \n", codes_first[i].bits, codes_first[i].code);
// }

// printf("%d \n", n);

// printf("po usunieciu \n");
// for(int i = 0; i < n; i++){
// 	printf("bits: %d Code: %d \n", codes_second[i].bits, codes_second[i].code);
// }

// printf("po stworzeniu kodow \n");

// for(int i = 0; i < n; i++){
// 	printf("bits: %d Code: %s o dlugosci kodu %d \n", codes[i].bits, codes[i].code, codes[i].code_length);
// }

// for(int i = 0; i < k; i++){
// 	printf("kod %d: %d\n", i, nodes[i].bits);
// }

// for(int i = 0; i < n; i++){
// 	printf("bits: %d Code: ", codes[i].bits);
// 	for(int j = 0; j < n; j++){
// 		if(codes[i].code[j] == 2)
// 			break;
// 		printf("%d", codes[i].code[j]);
// 	}
// 	printf("\n");
// }

// for(int i = 0; i < k; i++){
// 	printf("w wezle nr %d jest bits %d \n", i, nodes[i].bits);
// }
// printf("n = %d \n", n);
// printf("k = %d \n", k);



// for(int i = 0; i < k; i++){
// 	printf("Node at index %d: \n", i);
// 	printf("bits:%d ", nodes[i].bits);
// 	printf("frequency:%d ", nodes[i].frequency);
// 	if(nodes[i].left != NULL){
// 	    printf("Left child bits: %d ", nodes[i].left->bits);
// 	}
// 	else{
// 	    printf("Left child is NULL ");
// 	}
// 	if(nodes[i].right != NULL){
// 	    printf("Right child bits: %d ", nodes[i].right->bits);
// 	}
// 	else{
// 	    printf("Right child is NULL ");
// 	}
// 	printf("Been:%d ", nodes[i].been);
// 	printf("\n\n");
// }
