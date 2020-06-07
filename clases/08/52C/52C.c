#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <math.h>
#include <inttypes.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
//#define TAMANO(n) exp2l(ceill(log2l(n+1))+1)
#define TAMANO(n) 524288
#define LEN(arr) (size_t)(arr[0])


void construir_arbol(long long *arbol, long long *datos, size_t izquierda, size_t derecha, size_t pivote){
    if(izquierda == derecha){
        arbol[pivote] = datos[derecha-1];
        return;
    }

    size_t mitad = (izquierda + derecha) / 2;
    size_t pivote_izquierdo = 2*pivote;
    size_t pivote_derecho = 2*pivote + 1;

    construir_arbol(arbol, datos, izquierda, mitad, pivote_izquierdo);
    construir_arbol(arbol, datos, mitad+1, derecha, pivote_derecho);

    arbol[pivote] = MIN(arbol[pivote_izquierdo], arbol[pivote_derecho]);
}

void calcular_postergado(long long *arbol, long long *lazy, size_t pivote){
    if(lazy[pivote] == 0){
        return;
    }
    
    arbol[pivote] += lazy[pivote];

    size_t pivote_izquierdo = 2*pivote;
    size_t pivote_derecho = 2*pivote + 1;
    if(pivote_derecho < LEN(lazy)){
        lazy[pivote_izquierdo] += lazy[pivote];
        lazy[pivote_derecho] += lazy[pivote];
    }
    
    lazy[pivote] = 0;
}

void sumar_al_rango(long long *arbol, long long *lazy, size_t lf, size_t rg, long long v, size_t izquierda, size_t derecha, size_t pivote){
    calcular_postergado(arbol, lazy, pivote);

    if(rg < izquierda || derecha < lf){
        return;
    }

    if(lf <= izquierda && derecha <= rg){
        lazy[pivote] += v;
        calcular_postergado(arbol, lazy, pivote);
        return;
    }

    size_t mitad = (izquierda + derecha) / 2;
    size_t pivote_izquierdo = 2*pivote;
    size_t pivote_derecho = 2*pivote + 1;

    sumar_al_rango(arbol, lazy, lf, rg, v, izquierda, mitad, pivote_izquierdo);
    sumar_al_rango(arbol, lazy, lf, rg, v, mitad+1, derecha, pivote_derecho);

    arbol[pivote] = MIN(arbol[pivote_izquierdo], arbol[pivote_derecho]);
}

long long menor_en_rango(long long *arbol, long long *lazy, size_t lf, size_t rg, size_t izquierda, size_t derecha, size_t pivote){
    if(rg < izquierda || derecha < lf){
        return 1LL<<60;
    }

    calcular_postergado(arbol, lazy, pivote);
    if(lf <= izquierda && derecha <= rg){
        return arbol[pivote];
    }
    
    size_t mitad = (izquierda + derecha) / 2;
    size_t pivote_izquierdo = 2*pivote;
    size_t pivote_derecho = 2*pivote + 1;

    long long val_izq = menor_en_rango(arbol, lazy, lf, rg, izquierda, mitad, pivote_izquierdo);
    long long val_der = menor_en_rango(arbol, lazy, lf, rg, mitad+1, derecha, pivote_derecho);
    return MIN(val_izq, val_der);
}

int main(){
    size_t n;
    scanf("%zu", &n);

    size_t tamano_arbol = TAMANO(n);
    long long *pool = calloc(3*tamano_arbol, sizeof(long long));

    long long *arbol = pool;
    long long *lazy = &pool[tamano_arbol];
    arbol[0] = tamano_arbol;
    lazy[0] = tamano_arbol;

    long long *lista_a = &pool[2*tamano_arbol];
    for(size_t i = 0; i < n; ++i){
        scanf("%lld", &lista_a[i]);
    }

    construir_arbol(arbol, lista_a, 1, n, 1);

    int m;
    scanf("%d", &m);

    size_t lf, rg;
    long long v;
    while(m--){
        scanf("%zu %zu", &lf, &rg);
        ++lf;
        ++rg;
        char c;
        scanf("%c", &c);
        if(c != ' '){
            if(rg < lf){
                long long a = menor_en_rango(arbol, lazy, lf, n, 1, n, 1);
                long long b = menor_en_rango(arbol, lazy, 1, rg, 1, n, 1);
                printf("%lld\n", MIN(a, b));
            }
            else{
                printf("%lld\n", menor_en_rango(arbol, lazy, lf, rg, 1, n, 1));
            }
        }
        else{
            scanf("%lld", &v);
            if(rg < lf){
                sumar_al_rango(arbol, lazy, lf, n, v, 1, n, 1);
                sumar_al_rango(arbol, lazy, 1, rg, v, 1, n, 1);
            }
            else{
                sumar_al_rango(arbol, lazy, lf, rg, v, 1, n, 1);
            }
        }
    }

    //free(pool);
    return 0;
}
