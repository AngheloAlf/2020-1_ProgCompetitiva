#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <iostream>

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

int contar_platos(int n, const int calorias_plato[], int calorias_deseadas){
    int memo[CAL_MAX] = {0};
    memo[0] = 1;
    int acumulado = 0;
    for(int i = 0; i < n; ++i){
        int cal = calorias_plato[i];
        acumulado += cal;

        int j = acumulado;
        if(j > calorias_deseadas){
            j = calorias_deseadas;
        }
        while(j > 0 && j >= cal){
            int dsa = j - cal;
            memo[j] += memo[dsa];
            j -= 1;
        }
    }
    return memo[calorias_deseadas];
}

int main(){
    int T;
    //scanf("%d", &T);
    std::cin >> T;

    int calorias_plato[PLATOS_MAX];
    for(int i = 0; i < T; ++i){
        int N, M, K;
        std::cin >> N >> M >> K;
        //scanf("%d %d %d", &N, &M, &K);
        for(int j = 0; j < N; ++j){
            //scanf("%d", calorias_plato+j);
            std::cin >> calorias_plato[j];
        }
        qsort(calorias_plato, N, sizeof(int), intcmp);

        int recetas_posibles = contar_platos(N, calorias_plato, M);
        if(recetas_posibles >= K){
            //printf("ENOUGH\n");
            std::cout << "ENOUGH" << std::endl;
        }
        else{
            //printf("%d\n", recetas_posibles);
            std::cout << recetas_posibles << std::endl;
        }
    }

    return 0;
}
