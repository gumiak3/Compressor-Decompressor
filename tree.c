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

void make_tree(struct Input *tab, int n){
	Node* nodes = malloc(sizeof(Node)*(n*2));
	for(int i = 0; i < n; i++){
		nodes[i].symbol = tab[i].symbol;
		nodes[i].frequency = tab[i].frequency;
	}
	for(int i = 0; i < n*2; i++){
		nodes[i].bylo = false;
		nodes[i].left = NULL;
		nodes[i].right = NULL;
	}
	int k = 0;
	for(int i = 0; i < n; i++){
		if(nodes[i].bylo == false){
			k++;
		}
	}
	printf("%d \n", k);
	int mini1, mini2;
	while(k>1){
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
		printf("%d %d\n", mini1, mini2);
	
		nodes[n].frequency = nodes[mini1].frequency + nodes[mini2].frequency;
		nodes[n].left = &nodes[mini1];
		nodes[n].right = &nodes[mini2];
		nodes[mini1].bylo = true;
		nodes[mini2].bylo = true;
		n++;
		k = 0;
		for(int i = 0; i < n; i++){
			if(nodes[i].bylo == false){
				k++;
			}
		}
	}

	Node* new_nodes = realloc(nodes, sizeof(Node) * n);
	nodes = new_nodes;

	for(int i = 0; i < n; i++){
		printf("Node at index %d: \n", i);
		printf("Symbol:%d ", nodes[i].symbol);
		printf("frequency:%d ", nodes[i].frequency);
		if(nodes[i].left != NULL){
		    printf("Left child symbol: %d ", nodes[i].left->symbol);
		}
		else{
		    printf("Left child is NULL ");
		}
		if(nodes[i].right != NULL){
		    printf("Right child symbol: %d ", nodes[i].right->symbol);
		}
		else{
		    printf("Right child is NULL ");
		}
		printf("Bylo:%d ", nodes[i].bylo);
		printf("\n\n");
	}
	printf("Array length: %ld\n", sizeof(nodes) / sizeof(nodes[0]));
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
	make_tree(tab, n);

};
