#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef PLATOS_MAX
#define PLATOS_MAX 101
#endif
#ifndef CAL_MAX
#define CAL_MAX 10002
#endif

int intcmp(const void* a, const void* b){
    const int *c = (const int *)a;
    const int *d = (const int *)b;
    return *c-*d;
}

int contar_platos(int n, const int calorias_plato[n], int calorias_deseadas){
    if(calorias_deseadas < 0){
        return 0;
    }
    if(calorias_deseadas == 0){
        return 1;
    }

    int subcombinaciones = 0;
    for(int i = 0; i < n; ++i){
        int cal = calorias_plato[i];
        if(calorias_deseadas-cal < 0) break;
        subcombinaciones += contar_platos(n-1, calorias_plato + i+1, calorias_deseadas-cal);
    }
    return subcombinaciones;
}


int contar_platos_2_rec(int n, const int calorias_plato[n], int calorias_deseadas, int memo[CAL_MAX]){
    if(calorias_deseadas < 0){
        return 0;
    }
    if(calorias_deseadas == 0){
        return 0;
    }

    int subcombinaciones = 0;
    for(int i = 0; i < n; ++i){
        int cal = calorias_plato[i];
        int diff = calorias_deseadas-cal;
        if(diff < 0) break;
        if(memo[diff] == -1){
            memo[diff] = contar_platos_2_rec(n-1, calorias_plato + i+1, diff, memo);
        }
        else{
            memo[diff] += 1;
        }
        subcombinaciones += memo[diff];
    }
    //memo[calorias_deseadas] = subcombinaciones;
    return subcombinaciones;
}

int contar_platos_2(int n, const int calorias_plato[n], int calorias_deseadas){
    int memo[CAL_MAX] = {-1};
    memo[0] = 1;
    return contar_platos_2_rec(n, calorias_plato, calorias_deseadas, memo);
}

int main(){
    int T;
    scanf("%d", &T);

    int calorias_plato[PLATOS_MAX];
    for(int i = 0; i < T; ++i){
        int N, M, K;
        scanf("%d %d %d", &N, &M, &K);
        for(int j = 0; j < N; ++j){
            scanf("%d", calorias_plato+j);
        }
        qsort(calorias_plato, N, sizeof(int), intcmp);

        //int recetas_posibles = contar_platos_2(N, calorias_plato, M);
        int recetas_posibles = contar_platos(N, calorias_plato, M);
        if(recetas_posibles >= K){
            printf("ENOUGH\n");
            //printf("\t%d\n", recetas_posibles);
        }
        else{
            printf("%d\n", recetas_posibles);
        }
    }

    return 0;
}
