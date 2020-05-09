#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <queue>

#define TAMANO 64
#define GET_BIT(x, bit) (((x) >> (bit)) & 1LL)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define GRANDE 70


int main(){
    /// https://stackoverflow.com/a/12762166/6292472
    std::ios::sync_with_stdio(false);
    int n;
    std::cin >> n;

    long long int nodo;
    std::array<std::vector<long long int>, TAMANO> bits_por_nodo;

    for(int i = 0; i < n; ++i){
        std::cin >> nodo;

        if(nodo){
            for(unsigned j = 0; j < bits_por_nodo.size(); ++j){
                if(GET_BIT(nodo, j)){
                    bits_por_nodo[j].push_back(i);
                }
            }
        }
        else{
            --i;
            --n;
        }
    }

    std::unordered_map<int, std::vector<int>> grafo;
    bool tres_o_mas = false;
    bool al_menos_un_2 = false;
    for(unsigned i = 0; i < bits_por_nodo.size(); ++i){
        auto cantidad = bits_por_nodo[i].size();
        if(cantidad >= 3){
            tres_o_mas = true;
        }
        else if(cantidad == 2){
            al_menos_un_2 = true;

            auto x = bits_por_nodo[i][0];
            auto y = bits_por_nodo[i][1];

            if(grafo.count(x) == 0){
                grafo.insert({x, std::vector<int>()});
            }
            if(grafo.count(y) == 0){
                grafo.insert({y, std::vector<int>()});
            }
            grafo.at(x).push_back(y);
            grafo.at(y).push_back(x);
        }
    }

    if(tres_o_mas){
        std::cout << 3 << std::endl;
    }
    else if(!al_menos_un_2){
        std::cout << -1 << std::endl;
    }
    else{
        int nodos_ciclo = GRANDE;
        std::vector<int> padre;
        std::vector<int> distancias;
        for(int i = 0; i < n; ++i){
            padre.assign(n, -1);
            distancias.assign(n, GRANDE);
            distancias[i] = 0;
            std::queue<int> cola;
            cola.push(i);

            while(!cola.empty()){
                int nodo = cola.front();
                cola.pop();
                if(grafo.count(nodo)){
                    for(auto conectado: grafo.at(nodo)){
                        if(distancias[conectado] == GRANDE){
                            distancias[conectado] = distancias[nodo] + 1;
                            padre[conectado] = nodo;
                            cola.push(conectado);
                        }
                        else if(padre[nodo] != conectado && padre[conectado] != nodo){
                            nodos_ciclo = MIN(nodos_ciclo, distancias[nodo] + distancias[conectado] + 1);
                        }
                    }
                }
            }
        }

        std::cout << ((nodos_ciclo == GRANDE) ? -1 : nodos_ciclo) << std::endl;
    }

    return 0;
}
