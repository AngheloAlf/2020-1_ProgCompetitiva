#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef TAMANO
#define TAMANO 200003
#endif

int intcmp(const void* a__, const void* b__){
    int a = *(const int *)a__;
    int b = *(const int *)b__;
    return a-b;
}

void valor_rango(int n, int arr[n], int val){
    //col_seg_izq+col_b, col_b, col_c-col_b
    for(int i = 0; i < n; ++i){
        arr[i] = val;
    }
}

int direccion_mas_cercana(int col, int izq, int der){
    int di = col - izq;
    int dd = der - col;
    if(di == 0){
        return -1;
    }
    if(dd == 0){
        return 1;
    }
    if(di < 0){
        return 1;
    }
    if(dd < 0){
        return -1;
    }
    if(di <= dd){
        return -1;
    }
    return 1;
}

int ir_a_col_segura(int *pos, int col_act, int seguras_izq[], int seguras_der[]){
    if(direccion_mas_cercana(col_act, seguras_izq[col_act], seguras_der[col_act]) < 0){
        *pos = seguras_izq[col_act];
        return col_act - seguras_izq[col_act];
    }
    *pos = seguras_der[col_act];
    return seguras_der[col_act] - col_act;
}

int sacar_tesoros_en_fila(int *pos, int seguras_izq[], int seguras_der[], int tesoros_fila[2], bool ultima){
    int posizq = tesoros_fila[0];
    int posder = tesoros_fila[1];
    int pasos = 0;
    if(direccion_mas_cercana(*pos, posizq, posder) < 0){
        pasos += *pos - posizq;
        pasos += posder - posizq;
        if(!ultima){
            pasos += ir_a_col_segura(pos, posder, seguras_izq, seguras_der);
        }
    }
    else{
        pasos += posder - *pos;
        pasos += posder - posizq;
        if(!ultima){
            pasos += ir_a_col_segura(pos, posizq, seguras_izq, seguras_der);
        }
    }
    return pasos;
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
    int tesoros_por_fila[TAMANO] = {0};
    int tesoros_totales = 0;
    int tesoros[TAMANO][2] = {0};
    for(int i = 1; i < k+1; ++i){
        int a, b;
        scanf("%d %d", &a, &b);
        if(tesoros[a][0] == 0){
            tesoros[a][0] = b;
            tesoros[a][1] = b;
        }

        if(b < tesoros[a][0]){
            tesoros[a][0] = b;
        }
        if(b > tesoros[a][1]){
            tesoros[a][1] = b;
        }
        tesoros_por_fila[a] += b;
        tesoros_totales += b;
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

        valor_rango(col_c-col_b, col_seg_izq+col_b, col_b);
        valor_rango(col_c-col_b, col_seg_der+col_b+1, col_c);
    }
    // excepciones a la regla
    valor_rango(b[1]+1, col_seg_izq, b[1]);
    valor_rango(n-b[q]+1, col_seg_der + b[q]+1, b[q]);



    int y, x;
    y = 1;
    x = 1;
    int pasos = 0;
    for(; y < n+1 && tesoros_totales > 0; ++y){
        tesoros_totales -= tesoros_por_fila[y];
        bool ultima = tesoros_totales == 0 || y==n;
        pasos += sacar_tesoros_en_fila(&x, col_seg_izq, col_seg_der, tesoros[y], ultima);
        pasos += ultima ? 0 : 1;
    }

    printf("%d\n", pasos);


    return 0;
}
