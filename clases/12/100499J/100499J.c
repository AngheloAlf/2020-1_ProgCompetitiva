#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef TAMANO
#define TAMANO 101
#endif
#ifndef CAL_MAX
#define CAL_MAX 10002
#endif

#define MAX(x, y)   ((x) > (y) ? (x) : (y))

int intcmp(const void* a__, const void* b__){
    int a = *(const int *)a__;
    int b = *(const int *)b__;
    return a-b;
}
#if 0
int mochila_aux(int n, int calorias_deseadas, /*int recetas_deseadas,*/ const int calorias_plato[n]){
     // m[i]: cantidad de platos que suman i calorias.
    int m[CAL_MAX] = {0};
    //m[0] = 1;
    //m[calorias_plato[0]] = 1;
    for(int i = 0; i < n; ++i){
        int cal = calorias_plato[i];
        if(cal < calorias_deseadas){
            m[cal] += 1;
        }

        for(int j = 0; j < i; ++j){
            int cal2 = calorias_plato[j];
            if(cal2 + cal <= calorias_deseadas){
                m[cal2 + cal] += m[cal];
            }
        }
    }

    return m[calorias_deseadas];
}
#endif

#if 0
int mochila_aux(int n, int calorias_deseadas, /*int recetas_deseadas,*/ const int calorias_plato[n]){
    if(n == 0){
        return 0;
    }
    /*if(calorias_deseadas <= 0){
        return 0;
    }*/
    if(calorias_plato[n-1] > calorias_deseadas){
        return mochila_aux(n-1, calorias_deseadas, calorias_plato);
    }
    int cal = calorias_plato[n-1];
    int a = 1 + mochila_aux(n-1, calorias_deseadas-cal, calorias_plato);
    int b = mochila_aux(n-1, calorias_deseadas, calorias_plato);
    return MAX(a, b);
}
#endif

#if 0
int mochila_aux(int n, int calorias_deseadas, const int calorias_plato[n]){
    // m[i]: cantidad de platos que suman i calorias.
    int m[CAL_MAX] = {0};
    m[0] = 1;
    int cal_mayor = 0;
    for(int i = 0; i < n; ++i){
        int cal = calorias_plato[i];
        for(int cal2 = cal_mayor; cal2 >= 0; --cal2){
            int j = cal + cal2;
            if(j <= calorias_deseadas){
                m[j] += m[i];
                if(j > cal_mayor){
                    cal_mayor = j;
                }
            }
        }
    }
    return m[calorias_deseadas];
}

int mochila(int n, int calorias_deseadas, /*int recetas_deseadas,*/ const int calorias_plato[n]){
    int posibles = 0;
    while(n > 0){
        int platos = mochila_aux(n, calorias_deseadas, calorias_plato);
        if(platos > 0){
            //printf("%d\n", platos);
            posibles += 1;
        }
        n -= 1;
    }
    return posibles;
}
#endif


int rec(int n, int calorias_deseadas, const int calorias_plato[n], int count, int l, int *it){
    if(count == calorias_deseadas){
        *it += 1;
        return 1;
    }

    int asd = 0;
    for(int i = l; i < n; ++i){
        *it += 1;
        int cal = calorias_plato[i];
        if(count + cal > calorias_deseadas){
            continue;
        }
        asd += rec(n, calorias_deseadas, calorias_plato, count + cal, i+1, it);
    }
    return asd;
}
#if 0
int mochila_aux(int n, int calorias_deseadas, const int calorias_plato[n], int count, int l){
    if(count == calorias_deseadas){
        return 1;
    }

    int asd = 0;
    for(int i = l; i < n; ++i){
        int cal = calorias_plato[i];
        if(count + cal > calorias_deseadas){
            break;
        }
        printf("%d %d %d %d\n", l, i, count, cal);
        asd += mochila_aux(n, calorias_deseadas, calorias_plato, count + cal, i+1);
        printf("\t%d\n", asd);
    }
    return asd;
}
#endif
int mochila_aux(int n, const int calorias_plato[n], int calorias_deseadas){
    int memo[CAL_MAX] = {0};
    memo[0] = 1;
    int acumulado = 0;
    int it = 0;
    for(int i = 0; i < n; ++i){

        int cal = calorias_plato[i];
        //printf("\ncal[%d]: %d\n", i, cal);
        acumulado += cal;
        int j = acumulado;
        if(j > calorias_deseadas){
            j = calorias_deseadas;
        }
        while(j > 0 && j >= cal){
            it += 1;
            int dsa = j - cal;

            //m[j] += m[dsa];
            /*if(dsa < 0){
                break;
            }*/
            //printf("    %d\n", dsa);
            //if(dsa >= 0){
                //printf("    m[%d]: %d\n", dsa, memo[dsa]);
                //printf("    m[%d]: %d\n", j, memo[j]);
                memo[j] += memo[dsa];
                //printf("    m[%d]: %d\n", j, memo[j]);
                //printf("\n");
            //}

            j -= 1;
        }
    }
    printf("memo: %d\n", it);
    return memo[calorias_deseadas];
}

int cosito(int n, const int calorias_plato[n], int calorias_deseadas){
    int m[CAL_MAX] = {0};
    m[0] = 1;
    int currentSum = 0;
    for(int i = 0; i < n; i++){
        currentSum += calorias_plato[i];
        int j = currentSum;
        if(j > calorias_deseadas){
            j = calorias_deseadas;
        }
        for(; j >= calorias_plato[i]; j--){
            //printf("%d %d %d %d %d %d\n", i, j, calorias_plato[i], j - calorias_plato[i], m[j - calorias_plato[i]], m[j]);
            m[j] += m[j - calorias_plato[i]];
        }
    }
    for(int i = 0; i < calorias_deseadas+1; ++i){
        printf("%d: %d\n", i, m[i]);
    }
    return 0;
}

int mochila(int n, int calorias_deseadas, const int calorias_plato[n]){
    //return mochila_aux(n, calorias_deseadas, calorias_plato, 0);

    
    int zz = mochila_aux(n, calorias_plato, calorias_deseadas);
    printf("%d\n", zz);
    int it = 0;
    zz = rec(n, calorias_deseadas, calorias_plato, 0, 0, &it);
    printf("rec: %d\n", it);
    printf("%d\n", zz);
    exit(-1);
    return 0;
}


int main(){
    int T;
    scanf("%d", &T);

    int calorias_plato[TAMANO];
    for(int i = 0; i < T; ++i){
        int N, M, K;
        scanf("%d %d %d", &N, &M, &K);
        //calorias_plato[0] = 0;
        for(int j = 0; j < N; ++j){
            scanf("%d", calorias_plato+j);
        }
        qsort(calorias_plato, N, sizeof(int), intcmp);

        int recetas_posibles = mochila(N, M, /*K,*/ calorias_plato);
        if(recetas_posibles >= K){
            printf("ENOUGH\n");
        }
        else{
            printf("%d\n", recetas_posibles);
        }
    }


    return 0;
}