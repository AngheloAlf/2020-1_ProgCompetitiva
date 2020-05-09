#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#define MIN(x, y) ((x) < (y) ? (x) : (y))


int main(){
    std::ios::sync_with_stdio(false);
    int n;
    std::cin >> n;
    std::vector<long long int> numbers;
    numbers.reserve(n);
    //std::unordered_map<long long int, std::vector<long long int>> graph;

    /*long long int aux;
    for(int i = 0; i < n; ++i){
        std::cin >> aux;
        numbers[i] = aux;
    }*/

    
    //std::vector<long long int> bits_vect(64, 0);
    std::unordered_map<char, std::vector<char>> graph;
    //bits_vect.reserve(64);
    
    for(int i = 0; i < n; ++i){
        std::cin >> numbers[i];
        /*long long int aux;
        std::cin >> aux;
        numbers[i] = aux;*/
        /*for(int j = 0; j < 64; ++j){
            bits_vect[j] += (aux >> j) & 0x0001;
        }*/
    }

    for(int i = 0; i < n; ++i){
        long long int a = numbers[i];
        for(int j = i+1; j < n; ++j){
            long long int b = numbers[j];
            if(a & b){
                
                for(int x = 0; x < 64; ++x){
                    if((a >> x) & 0x0001){
                        for(int y = 0; y < 64; ++y){
                            if((b >> y) & 0x0001){
                                if(graph.count(x) == 0){
                                    //std::unordered_set<char> aux;
                                    //graph.insert(std::make_pair<char, std::unordered_set<char>>(a_bit, aux));
                                    graph.insert({x, std::vector<char>()});
                                }
                                if(graph.count(y) == 0){
                                    //std::unordered_set<char> aux;
                                    //graph.insert(std::make_pair<char, std::unordered_set<char>>(b_bit, aux));
                                    graph.insert({y, std::vector<char>()});
                                }
                                graph.at(x).push_back(y);
                                graph.at(y).push_back(x);
                            }
                        }
                    }
                }
            }
        }
    }

    for(auto asdf: graph){
        std::cout << (int)asdf.first << ": ";
        for(auto fdsa: asdf.second){
            std::cout << (int)fdsa << " ";
        }
        std::cout << std::endl;
    }

    return 0;

    int nodos_ciclo = 127;
    for(int i = 0; i < 64; ++i){
        if(graph.count(i) == 0){
            continue;
        }
        std::vector<int> distancias(64, 127);
        distancias[i] = 0;
        std::vector<int> padre(64, -1);
        std::queue<int> cola;
        cola.push(i);

        while(!cola.empty()){
            int nodo = cola.front();
            cola.pop();
            if(graph.count(nodo)){
                auto nodos_destinos = graph.at(nodo);
                for(char conectado: nodos_destinos){
                    if(distancias[conectado] == 127){
                        distancias[conectado] = distancias[nodo]+1;
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

    if(nodos_ciclo == 127){
        std::cout << -1 << std::endl;
    }
    else{
        std::cout << nodos_ciclo << std::endl;
    }

    /*for(int j = 0; j < 64; ++j){
        if(bits_vect.at(j))
            std::cout << j << ": " << bits_vect.at(j) << std::endl;
    }*/
    //std::cout << bits_vect << std::endl;
    return 0;
/*
    for(int i = 0; i < n; ++i){
        long long int a = numbers[i];
        for(int j = i+1; j < n; ++j){
            long long int b = numbers[j];
            if(a & b){
                if(graph.count(i) == 0){
                    graph.insert(std::make_pair<long long int, std::vector<long long int>>(i, std::vector<long long int>()));
                }
                if(graph.count(j) == 0){
                    graph.insert(std::make_pair<long long int, std::vector<long long int>>(j, std::vector<long long int>()));
                }
                graph.at(i).push_back(j);
                graph.at(j).push_back(i);
            }
        }
    }

    long long int nodos_ciclo = 1LL << 62;
    for(int i = 0; i < n; ++i){
        std::vector<int> distancias(n, 1 << 30);
        distancias[i] = 0;
        std::vector<int> padre(n, -1);
        std::queue<int> cola;
        cola.push(i);

        while(!cola.empty()){
            int nodo = cola.front();
            cola.pop();
            if(graph.count(nodo)){
                for(long long int conectado: graph.at(nodo)){
                    if(distancias[conectado] == (1 << 30)){
                        distancias[conectado] = distancias[nodo]+1;
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

    if(nodos_ciclo == 1LL << 62){
        std::cout << -1 << std::endl;
    }
    else{
        std::cout << nodos_ciclo << std::endl;
    }

    return 0;*/
}
