#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>

typedef struct{
    uint64_t left, right, diff;
} CutPair;


CutPair *CutPair_init(uint64_t left, uint64_t right){
    assert(left < right);
    CutPair *pair = malloc(sizeof(CutPair));
    pair->left = left;
    pair->right = right;
    pair->diff = right-left;
    return pair;
}
void CutPair_free(CutPair *pair){
    free(pair);
}

bool CutPair_in_between(CutPair *pair, uint64_t cut){
    return pair->left < cut && cut < pair->right;
}

void CutPair_cut_pair(CutPair *src_pair, uint64_t cut, CutPair **dst_left, CutPair **dst_right){
    assert(CutPair_in_between(src_pair, cut));
    *dst_left = CutPair_init(src_pair->left, cut);
    *dst_right = CutPair_init(cut, src_pair->right);
}


typedef struct{
    CutPair **cuts_list;
    CutPair **widths_stack;
    size_t size, len;
} CutsList;


CutsList *CutsList_init(size_t elements_amount, uint64_t left, uint64_t right){
    CutsList *list = malloc(sizeof(CutsList));
    list->size = elements_amount;
    list->cuts_list = calloc(2*elements_amount, sizeof(CutPair *));
    list->widths_stack = &list->cuts_list[elements_amount];

    list->cuts_list[0] = CutPair_init(left, right);
    list->widths_stack[0] = list->cuts_list[0];
    list->len = 1;
    return list;
}
void CutsList_free(CutsList *list){
    for(size_t i = 0; i < list->len; ++i){
        CutPair_free(list->cuts_list[i]);
    }
    free(list->cuts_list);
    free(list);
}

int64_t CutsList_biggest_diff(CutsList *list){
    return list->widths_stack[0]->diff;
}

void CutsList_add_cut(CutsList *list, uint64_t cut){
    assert(list->len < list->size);

    CutPair *old_pair;
    CutPair *dst_left;
    CutPair *dst_right;

    if(list->len == 1){
        assert(CutPair_in_between(list->cuts_list[0], cut));

        old_pair = list->cuts_list[0];
        CutPair_cut_pair(old_pair, cut, &dst_left, &dst_right);

        list->cuts_list[0] = dst_left;
        list->cuts_list[1] = dst_right;

        if(dst_left->diff >= dst_right->diff){
            list->widths_stack[0] = dst_left;
            list->widths_stack[1] = dst_right;
        }
        else{
            list->widths_stack[0] = dst_right;
            list->widths_stack[1] = dst_left;
        }
        CutPair_free(old_pair);
        ++list->len;
        return;
    }

    bool cut_inserted = false;
    for(size_t i = 0; i < list->len && !cut_inserted; ++i){
        if(CutPair_in_between(list->cuts_list[i], cut)){
            old_pair = list->cuts_list[i];
            CutPair_cut_pair(old_pair, cut, &dst_left, &dst_right);

            memmove(&list->cuts_list[i+2], &list->cuts_list[i+1], (list->len-i-1)*sizeof(CutPair *));
            list->cuts_list[i] = dst_left;
            list->cuts_list[i+1] = dst_right;
            cut_inserted = true;
        }
    }
    assert(cut_inserted);

    bool width_removed = false;
    for(size_t i = 0; i < list->len && !width_removed; ++i){
        if(list->widths_stack[i] == old_pair){
            memmove(&list->widths_stack[i], &list->widths_stack[i+1], (list->len-i-1)*sizeof(CutPair *));
            CutPair_free(old_pair);
            width_removed = true;
        }
    }
    assert(width_removed);
    
    bool left_inserted = false;
    bool right_inserted = false;
    CutPair *biggest = list->widths_stack[0];
    if(dst_left->diff >= biggest->diff || dst_right->diff >= biggest->diff){
        if(dst_left->diff >= dst_right->diff && dst_right->diff >= biggest->diff){
            memmove(&list->widths_stack[2], list->widths_stack, (list->len - 1)*sizeof(CutPair *));
            list->widths_stack[0] = dst_left;
            left_inserted = true;
            list->widths_stack[1] = dst_right;
            right_inserted = true;
        }
        else if(dst_right->diff >= dst_left->diff && dst_left->diff >= biggest->diff){
            memmove(&list->widths_stack[2], list->widths_stack, (list->len - 1)*sizeof(CutPair *));
            list->widths_stack[0] = dst_right;
            right_inserted = true;
            list->widths_stack[1] = dst_left;
            left_inserted = true;
        }
        else if(dst_left->diff >= biggest->diff){
            memmove(&list->widths_stack[1], list->widths_stack, (list->len - 1)*sizeof(CutPair *));
            list->widths_stack[0] = dst_left;
            left_inserted = true;
        }
        else if(dst_right->diff >= biggest->diff){
            memmove(&list->widths_stack[1], list->widths_stack, (list->len - 1)*sizeof(CutPair *));
            list->widths_stack[0] = dst_right;
            right_inserted = true;
        }
    }

    for(long i = list->len-1; i >= 1 && (!left_inserted || !right_inserted); --i){
        if(!left_inserted && list->widths_stack[i-1]->diff >= dst_left->diff){
            memmove(&list->widths_stack[i+1], &list->widths_stack[i], (list->len - i - (right_inserted?0:1))*sizeof(CutPair *));
            list->widths_stack[i] = dst_left;
            left_inserted = true;
        }
        if(!right_inserted && list->widths_stack[i-1]->diff >= dst_right->diff){
            memmove(&list->widths_stack[i+1], &list->widths_stack[i], (list->len - i - (left_inserted?0:1))*sizeof(CutPair *));
            list->widths_stack[i] = dst_right;
            right_inserted = true;
        }
    }
    assert(left_inserted);
    assert(right_inserted);
    ++list->len;
}



int main(){
    long w, h, n;
    scanf("%li %li %li", &w, &h, &n);
    CutsList *h_cuts = CutsList_init(n+1, 0, h);
    CutsList *v_cuts = CutsList_init(n+1, 0, w);

    char direction;
    uint64_t distance;
    uint64_t last_big_h = h;
    uint64_t last_big_v = w;
    for(long i = 0; i < n; ++i){
        scanf(" %c %"SCNu64, &direction, &distance);

        if(direction == 'H'){
            CutsList_add_cut(h_cuts, distance);
            last_big_h = CutsList_biggest_diff(h_cuts);
        }
        else{
            CutsList_add_cut(v_cuts, distance);
            last_big_v = CutsList_biggest_diff(v_cuts);
        }
        printf("%"PRIu64"\n", last_big_h*last_big_v);
    }


    CutsList_free(h_cuts);
    CutsList_free(v_cuts);

    return 0;
}
