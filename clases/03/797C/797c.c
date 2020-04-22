#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/// Encuentra el menor elemento en `string` y cuantas veces se repite.
char min_val(const char *string, size_t len, long *amount){
    char val = 127;
    for(size_t i = 0; i < len; ++i){
        if(string[i] < val){
            val = string[i];
            *amount = 1;
        }
        else if(string[i] == val){
            *amount += 1;
        }
    }
    return val;
}

int main(){
    char *pool = calloc(300003, sizeof(char));

    char *s = &pool[0];
    char *t = &pool[100001];
    char *u = &pool[200002];

    scanf("%s", s);

    size_t s_len = strlen(s);
    size_t t_len = 0;
    size_t u_len = 0;

    long amount = 0;
    char minimal;
    while(s_len || t_len){
        /// Buscamos el nuevo valor mínimo si ya no existe el ultimo dentro del string.
        if(amount <= 0){
            minimal = min_val(s, s_len, &amount);
        }

        /// Si el último valor en `t` es menor al minimo actual en `s`, 
        /// lo agregamos a `u`, con el fin de intentar ordenar lexiograficamente.
        if(s_len && t_len && (t[t_len-1] <= minimal)){
            u[u_len] = t[t_len-1];
            ++u_len;
            t[t_len-1] = 0;
            --t_len;
        }
        else if(s_len){
            t[t_len] = s[0];
            ++t_len;
            if(s[0] == minimal){
                --amount;
            }
            ++s;
            --s_len;
        }
        else{
            u[u_len] = t[t_len-1];
            ++u_len;
            t[t_len-1] = 0;
            --t_len;
        }
    }

    printf("%s\n", u);
    free(pool);
    return 0;
}
