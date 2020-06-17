#include <iostream>
#include <vector>
#include <cmath>
#include <assert.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define TAMANO(n) std::exp2l(std::ceil(std::log2l(n+1))+1)


void imprimir_arreglo(const std::vector<long long> &arreglo){
    std::cout << "{";
    bool primero = true;
    for(size_t i = 0; i < arreglo.size(); ++i){
        if(arreglo[i]){
            if(!primero){
                std::cout << ", ";
            }
            else{
                primero = false;
            }
            std::cout << arreglo[i];
        }
    }
    std::cout << "}" << std::endl;
}

void construir_arbol(std::vector<long long> &arbol, const std::vector<long long> &arreglo, size_t izquierda, size_t derecha, size_t pivote){
    if(izquierda == derecha){
        arbol[pivote] = arreglo[derecha];
        return;
    }

    size_t mitad = (izquierda + derecha) / 2;
    size_t pivote_izquierdo = 2*pivote;
    size_t pivote_derecho = 2*pivote + 1;

    construir_arbol(arbol, arreglo, izquierda, mitad, pivote_izquierdo);
    construir_arbol(arbol, arreglo, mitad+1, derecha, pivote_derecho);

    arbol[pivote] = arbol[pivote_izquierdo] + arbol[pivote_derecho];
}

void construir_arbol(std::vector<long long> &arbol, const std::vector<long long> &arreglo){
    construir_arbol(arbol, arreglo, 1, arreglo.size()-1, 1);
}

void actualizar_k(std::vector<long long> &arbol, std::vector<long long> &arreglo, size_t k, bool insertar, size_t izquierda, size_t derecha, size_t pivote){
    if(k == izquierda && derecha == k){
        arreglo[k] += insertar ? 1 : -1;
        arbol[pivote] = arreglo[k];
        return;
    }

    if(k < izquierda || derecha < k){
        return;
    }

    size_t mitad = (izquierda + derecha) / 2;
    size_t pivote_izquierdo = 2*pivote;
    size_t pivote_derecho = 2*pivote + 1;

    actualizar_k(arbol, arreglo, k, insertar, izquierda, mitad, pivote_izquierdo);
    actualizar_k(arbol, arreglo, k, insertar, mitad+1, derecha, pivote_derecho);

    arbol[pivote] = arbol[pivote_izquierdo] + arbol[pivote_derecho];
}
void actualizar_k(std::vector<long long> &arbol, std::vector<long long> &arreglo, long long k, bool insertar){
    actualizar_k(arbol, arreglo, k, insertar, 1, arreglo.size()-1, 1);
}

ssize_t encontrar_posicion_k(std::vector<long long> &arbol, std::vector<long long> &arreglo, size_t k, size_t izquierda, size_t derecha, size_t pivote, size_t fw){
    if(arbol[pivote] + fw < k){
        return -1;
    }
    
    size_t mitad = (izquierda + derecha) / 2;
    if(izquierda >= derecha){
        return mitad;
    }
    
    size_t pivote_izquierdo = 2*pivote;
    size_t pivote_derecho = 2*pivote + 1;

    if(arbol[pivote_izquierdo] + fw >= k){
        return encontrar_posicion_k(arbol, arreglo, k, izquierda, mitad, pivote_izquierdo, fw);
    }
    return encontrar_posicion_k(arbol, arreglo, k, mitad+1, derecha, pivote_derecho, arbol[pivote_izquierdo] + fw);
}
void remover_k_esimo(std::vector<long long> &arbol, std::vector<long long> &arreglo, size_t k){
    ssize_t pos = encontrar_posicion_k(arbol, arreglo, k, 1, arreglo.size()-1, 1, 0);
    //std::cout << pos << std::endl;
    assert(pos >= 0);
    assert(pos != 0);
    actualizar_k(arbol, arreglo, pos, false);
}

long long encontrar_algun_elemento(const std::vector<long long> &arreglo){
    for(size_t i = 1; i < arreglo.size(); ++i){
        if(arreglo[i] > 0){
            return i;
        }
    }
    return 0;
}


int main(){
    std::ios::sync_with_stdio(false);

    size_t n, q;
    std::cin >> n >> q;

    /// Arreglo que indica cuantas veces está repetido el número en el indice `i`.
    std::vector<long long> arreglo;
    arreglo.resize(n+1, 0);

    size_t tamano_arbol = TAMANO(n+1);
    std::vector<long long> arbol;
    arbol.resize(tamano_arbol, 0);

    long long x;
    for(size_t i = 0; i < n; ++i){
        std::cin >> x;
        ++arreglo[x];
    }

    construir_arbol(arbol, arreglo);
    //imprimir_arreglo(arbol);

    long long k;
    while(q--){
        std::cin >> k;
        //std::cout << std::endl << k << std::endl;
        //imprimir_arreglo(arbol);
        if(k < 0){
            remover_k_esimo(arbol, arreglo, -k);
        }
        else{
            actualizar_k(arbol, arreglo, k, true);
        }
    }
    //imprimir_arreglo(arreglo);
    std::cout << encontrar_algun_elemento(arreglo) << std::endl;

    return 0;
}
