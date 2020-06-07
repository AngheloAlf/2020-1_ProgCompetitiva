#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define TAMANO(n) std::exp2l(std::ceil(std::log2l(n+1))+1)


void construir_arbol(std::vector<long long> &arbol, const std::vector<long long> &datos, size_t izquierda, size_t derecha, size_t pivote){
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

void calcular_postergado(std::vector<long long> &arbol, std::vector<long long> &lazy, size_t pivote){
    if(lazy[pivote] == 0){
        return;
    }
    
    arbol[pivote] += lazy[pivote];

    size_t pivote_izquierdo = 2*pivote;
    size_t pivote_derecho = 2*pivote + 1;
    if(pivote_derecho < lazy.size()){
        lazy[pivote_izquierdo] += lazy[pivote];
        lazy[pivote_derecho] += lazy[pivote];
    }
    
    lazy[pivote] = 0;
}

void sumar_al_rango(std::vector<long long> &arbol, std::vector<long long> &lazy, size_t lf, size_t rg, long long v, size_t izquierda, size_t derecha, size_t pivote){
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

long long menor_en_rango(std::vector<long long> &arbol, std::vector<long long> &lazy, size_t lf, size_t rg, size_t izquierda, size_t derecha, size_t pivote){
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

int leer_numeros(size_t &lf, size_t &rg, long long &v){
    std::string line;
    std::getline(std::cin, line); 

    std::istringstream is(line);
    long long number;
    int i = 0;
    while(is >> number){
        switch(i){
            case 0:
                lf = number;
                break;
            case 1:
                rg = number;
                break;
            case 2:
                v = number;
                break;
            default:
                break;
        }
        ++i;
    }
    return i;
}


int main(){
    std::ios::sync_with_stdio(false);

    size_t n;
    std::cin >> n;

    size_t tamano_arbol = TAMANO(n);

    std::vector<long long> lista_a;
    lista_a.resize(n, 0);
    std::vector<long long> arbol;
    arbol.resize(tamano_arbol, 0);
    std::vector<long long> lazy;
    lazy.resize(tamano_arbol, 0);

    for(size_t i = 0; i < n; ++i){
        std::cin >> lista_a[i];
    }

    construir_arbol(arbol, lista_a, 1, n, 1);

    long long m;
    std::cin >> m;
    std::cin.ignore();

    size_t lf, rg;
    long long v;
    while(m--){
        int i = leer_numeros(lf, rg, v);
        ++lf;
        ++rg;
        if(i == 2){
            if(rg < lf){
                long long a = menor_en_rango(arbol, lazy, lf, n, 1, n, 1);
                long long b = menor_en_rango(arbol, lazy, 1, rg, 1, n, 1);
                std::cout << MIN(a, b) << std::endl;
            }
            else{
                std::cout << menor_en_rango(arbol, lazy, lf, rg, 1, n, 1) << std::endl;
            }
        }
        else{
            if(rg < lf){
                sumar_al_rango(arbol, lazy, lf, n, v, 1, n, 1);
                sumar_al_rango(arbol, lazy, 1, rg, v, 1, n, 1);
            }
            else{
                sumar_al_rango(arbol, lazy, lf, rg, v, 1, n, 1);
            }
        }
    }

    return 0;
}
