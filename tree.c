#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char symbol[9];
    int frequency;
    struct Node* left;
    struct Node* right;
};

int cmp(const void* a, const void* b) {
    const char* const *pa = a;
    const char* const *pb = b;
    return strcmp(*pa, *pb);
}

int main() {
    char s[1001];
    scanf("%s", s);
    int n = strlen(s)/8, k = 0;
    char** v = (char**)malloc(n * sizeof(char*));
    for(int i=0; i<n; i++){
        v[i] = (char*)malloc(9 * sizeof(char));
        for(int j=0; j<8; j++){
            v[i][j] = s[i*8+j];
        }
        v[i][8] = '\0';
    }
 
    qsort(v, n, sizeof(char*), cmp);
    for(int i=0; i<=n - 1; i++)
        if(strcmp(v[i], v[i+1]) != 0)
            k++;
    
    char** w = (char**)malloc(k * sizeof(char*));
    for(int i=0; i<k; i++){
        w[i] = (char*)malloc(9 * sizeof(char));
    }

    int* f = (int*)malloc(k * sizeof(int));
    w[0] = v[0];
    f[0] = 1;
    int j = 0;
    for(int i=1; i<n; i++){
        if(strcmp(v[i], v[i-1]) == 0){
            f[j]++;
        }
        else{
            j++;
            w[j] = v[i];
            f[j] = 1;
        }
    }
    struct Node* tree = (struct Node*)malloc(k * sizeof(struct Node));
    for(int i=0; i<k; i++){
        strcpy(tree[i].symbol, w[i]);
        tree[i].frequency = f[i];
        tree[i].left = NULL;
        tree[i].right = NULL;
    }

    for(int i=0; i<k; i++){
        printf("%s %d\n", w[i], f[i]);
    }

    printf("%d\n", k);
    for(int i=0; i<n; i++){
        printf("%s ", v[i]);
    }
    
    for(int i=0; i<n; i++){
        free(v[i]);
    }
    free(v);

    for(int i=0; i<k; i++){
        free(w[i]);
    }
    free(w);

    free(f);

    free(tree);

    return 0;
}
