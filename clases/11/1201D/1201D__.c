#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAMANO 200003

int intcmp(const void* a__, const void* b__){
    int a = *(const int *)a__;
    int b = *(const int *)b__;
    return a-b;
}

void minmax(int *min, int *max, int tesoros_fila[]){
    *min = TAMANO;
    *max = 0;
    for(int i = 1; tesoros_fila[i]; ++i){
        if(tesoros_fila[i] < *min){
            *min = tesoros_fila[i];
        }
        if(tesoros_fila[i] > *max){
            *max = tesoros_fila[i];
        }
    }
}

int sacar_tesoros_en_fila(int *pos, int m, int q, int seguras_izq[q+2], int seguras_der[q+2], int tesoros_fila[]){
    int pos_tes_izq;
    int pos_tes_der;
    minmax(&pos_tes_izq, &pos_tes_der, tesoros_fila);

    
}


int main(){
    int n, m, k, q;
    scanf("%d %d %d %d", &n, &m, &k, &q);


    /*
    int r[k];
    int c[k];
    for(int i = 0; i < k; ++i){
        scanf("%d %d", r+i, c+i);
    }
    //qsort(r, k, sizeof(int), intcmp);
    //qsort(c, k, sizeof(int), intcmp);
    */

    // tesoros[i]: arreglo con los tesoros de la fila i. El arreglo está terminado en cero.
    // tesoros[i][j]: columna donde está ubicado el j-esimo tesoso de la fila i.
    int *tesoros[TAMANO];
    int *tesoros_aux[TAMANO];
    for(int i = 1; i < n+1; ++i){
        tesoros[i] = tesoros_aux[i] = calloc(k+2, sizeof(int));
    }
    for(int i = 1; i < k+1; ++i){
        int a, b;
        scanf("%d %d", &a, &b);
        *tesoros_aux[a] = b;
        ++tesoros_aux[a];
    }
    

    int b[TAMANO];
    for(int i = 1; i < q+1; ++i){
        scanf("%d", b+i);
    }
    qsort(b+1, q, sizeof(int), intcmp);
    b[0] = 0;
    b[q+1] = n+1;

    // col_seg_izq[i]: la columna segura más cercana a la izquierda de la col i.
    int col_seg_izq[TAMANO];
    // col_seg_der[i]: la columna segura más cercana a la derecha de la col i.
    int col_seg_der[TAMANO];
    /*for(int j = 1; j < n+1; ++j){

    }*/
    for(int i = 0; i < q+1; ++i){
        //int col_a = b[i-1];
        int col_b = b[i];
        int col_c = b[i+1];

        memset(col_seg_izq+col_b, col_b, col_c-col_b);
        memset(col_seg_der+col_b+1, col_c, col_c-col_b);
    }
    // excepciones a la regla
    memset(col_seg_izq, b[1], b[1]+1);
    memset(col_seg_der + b[q]+1, b[q], n-b[q]+1);



    int y, x;
    y = 1;
    x = 1;





    return 0;
}
