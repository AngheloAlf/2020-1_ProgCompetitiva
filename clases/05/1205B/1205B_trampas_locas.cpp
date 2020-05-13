#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
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

    std::unordered_set<int> solo_un_elemento;
    std::unordered_map<int, std::unordered_set<int>> grafo;
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
                grafo.insert({x, std::unordered_set<int>()});
                grafo.at(x).insert(y);
                solo_un_elemento.insert(x);
            }
            else{
                grafo.at(x).insert(y);
                solo_un_elemento.erase(x);
            }
            if(grafo.count(y) == 0){
                grafo.insert({y, std::unordered_set<int>()});
                grafo.at(y).insert(x);
                solo_un_elemento.insert(y);
            }
            else{
                grafo.at(y).insert(x);
                solo_un_elemento.erase(y);
            }
        }
    }

    //std::cout << n << std::endl;

    /*for(auto cosito: grafo){
        std::cout << cosito.first << ":";
        for(auto cosito2: cosito.second){
            std::cout << " " << cosito2;
        }
        std::cout << std::endl;
    }
    return 0;*/

    if(tres_o_mas){
        std::cout << 3 << std::endl;
    }
    else if(!al_menos_un_2){
        std::cout << -1 << std::endl;
    }
    else{
        std::unordered_set<int> solo_un_elemento_aux;
        while(!solo_un_elemento.empty()){
            for(int nodo_solito: solo_un_elemento){
                auto conexiones_solito = grafo.find(nodo_solito);
                if(conexiones_solito == grafo.end()){
                    continue;
                }
                int destino_del_solito = *conexiones_solito->second.begin();
                auto conexiones_del_destino = grafo.find(destino_del_solito);
                conexiones_del_destino->second.erase(nodo_solito);
                switch(conexiones_del_destino->second.size()){
                    case 1:
                        solo_un_elemento_aux.insert(destino_del_solito);
                        break;
                    case 0:
                        grafo.erase(conexiones_del_destino);
                        break;
                }
                grafo.erase(conexiones_solito);
            }
            solo_un_elemento = solo_un_elemento_aux;
            solo_un_elemento_aux.clear();
        }
        
        std::cout << grafo.size() << std::endl;

        /*for(auto cosito: grafo){
            std::cout << cosito.first << ":";
            for(auto cosito2: cosito.second){
                std::cout << " " << cosito2;
            }
            std::cout << std::endl;
        }
        return 0;*/

        /*int nodos_ciclo = GRANDE;
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

        std::cout << ((nodos_ciclo == GRANDE) ? -1 : nodos_ciclo) << std::endl;*/
    }

    return 0;
}
