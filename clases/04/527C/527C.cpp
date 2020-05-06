#include <iostream>
#include <set>
#include <cstdint>
#include <vector>
#include <algorithm>


int main(){
    int64_t w, h, n;
    std::cin >> w >> h >> n;

    std::set<int64_t> H_cuts;
    std::set<int64_t> V_cuts;
    H_cuts.insert(0);
    H_cuts.insert(h);
    V_cuts.insert(0);
    V_cuts.insert(w);

    int64_t last_big_H = h;
    int64_t last_big_V = w;

    std::vector<int64_t> H_heap {h};
    std::vector<int64_t> V_heap {w};

    std::string direction;
    int64_t distance;
    int64_t diff;
    int64_t big_H, H_lower = 0, H_upper = h;
    int64_t big_V, V_lower = 0, V_upper = w;
    for(int64_t i = 0; i < n; ++i){
        std::cin >> direction >> distance;

        if(direction == "H"){

            auto it = H_cuts.lower_bound(distance);
            int64_t upper = *it;
            int64_t lower = *(--it);

            H_heap.erase(find(H_heap.begin(), H_heap.end(), upper-lower));

            H_heap.push_back(distance-lower);
            H_heap.push_back(upper-distance);
            std::pop_heap(H_heap.begin(), H_heap.end());
            last_big_H = H_heap.back();
            std::push_heap(H_heap.begin(), H_heap.end());

            H_cuts.insert(distance);

            /*if(distance > H_lower && distance < H_upper){
            }*/

            /*if(distance > H_lower && distance < H_upper){
                auto first = H_cuts.begin();
                auto after = H_cuts.begin();
                ++after;
                big_H = 0;
                for(; after != H_cuts.end(); ++first, ++after){
                    diff = *after - *first;
                    if(diff > big_H){
                        big_H = diff;
                        H_lower = *first;
                        H_upper = *after;
                        if(big_H == last_big_H){
                            break;
                        }
                    }
                }
                last_big_H = big_H;
            }*/

        }
        else{
            auto it = V_cuts.lower_bound(distance);
            int64_t upper = *it;
            int64_t lower = *(--it);

            V_heap.erase(find(V_heap.begin(), V_heap.end(), upper-lower));

            V_heap.push_back(distance-lower);
            V_heap.push_back(upper-distance);
            make_heap(V_heap.begin(), V_heap.end());
            pop_heap(V_heap.begin(), V_heap.end());
            last_big_V = V_heap.back();
            push_heap(V_heap.begin(), V_heap.end());

            V_cuts.insert(distance);
/*
            V_cuts.insert(distance);

            if(distance > V_lower && distance < V_upper){
                auto first = V_cuts.begin();
                auto after = V_cuts.begin();
                ++after;
                big_V = 0;
                for(; after != V_cuts.end(); ++first, ++after){
                    diff = *after - *first;
                    if(diff > big_V){
                        big_V = diff;
                        V_lower = *first;
                        V_upper = *after;
                        if(big_V == last_big_V){
                            break;
                        }
                    }
                }
                last_big_V = big_V;
            }
*/
        }
        std::cout << (last_big_H*last_big_V) << std::endl;
    }
    return 0;
}

