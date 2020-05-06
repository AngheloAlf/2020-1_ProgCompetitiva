#include <iostream>
#include <set>
#include <cstdint>
#include <vector>
#include <algorithm>


int main(){
    int64_t w, h, n;
    std::cin >> w >> h >> n;

    std::multiset<int64_t> H_cuts;
    std::multiset<int64_t> V_cuts;
    H_cuts.insert(0);
    H_cuts.insert(h);
    V_cuts.insert(0);
    V_cuts.insert(w);

    int64_t last_big_H = h;
    int64_t last_big_V = w;

    std::multiset<int64_t> H_width;
    std::multiset<int64_t> V_width;
    H_width.insert(h);
    V_width.insert(w);

    std::string direction;
    int64_t distance;
    for(int64_t i = 0; i < n; ++i){
        std::cin >> direction >> distance;

        if(direction == "H"){
            auto it = H_cuts.lower_bound(distance);
            int64_t upper = *it;
            int64_t lower = *(--it);
            H_width.erase(H_width.find(upper-lower));

            H_width.insert(upper-distance);
            H_width.insert(distance-lower);
            last_big_H = *H_width.rbegin();
            H_cuts.insert(distance);
        }
        else{
            auto it = V_cuts.lower_bound(distance);
            int64_t upper = *it;
            int64_t lower = *(--it);
            V_width.erase(V_width.find(upper-lower));

            V_width.insert(upper-distance);
            V_width.insert(distance-lower);
            last_big_V = *V_width.rbegin();
            V_cuts.insert(distance);
        }
        std::cout << (last_big_H*last_big_V) << std::endl;
    }
    return 0;
}

