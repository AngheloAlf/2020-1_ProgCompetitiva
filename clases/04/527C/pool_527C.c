#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
//#define NDEBUG 
#include <assert.h>


void *_mem_pool = NULL;
size_t _mem_size = 0;
size_t _mem_pos = 0;

void MemPool_init(size_t size){
    assert(_mem_pool == NULL);
    _mem_pool = malloc(size);
    _mem_size = size;
}
void MemPool_free(){
    assert(_mem_pool != NULL);
    free(_mem_pool);
    _mem_pool = NULL;
    _mem_size = 0;
    _mem_pos = 0;
}

void *MemPool_alloc(size_t size){
    assert(_mem_pool != NULL);
    //printf("%zu + %zu = %zu <= %zu\n", _mem_pos, size, _mem_pos + size, _mem_size);
    if(!(_mem_pos + size <= _mem_size)){
        fprintf(stderr, "Error: %zu + %zu = %zu <= %zu\n", _mem_pos, size, _mem_pos + size, _mem_size);
    }
    assert(_mem_pos + size <= _mem_size);
    void *mem = _mem_pool + _mem_pos;
    _mem_pos += size;
    return mem;
}


typedef struct{
    uint64_t left, right, diff;
} CutPair;


size_t CutPair_mem(){
    return sizeof(CutPair);
}
CutPair *CutPair_init(uint64_t left, uint64_t right){
    assert(left < right);
    CutPair *pair = MemPool_alloc(sizeof(CutPair));
    pair->left = left;
    pair->right = right;
    pair->diff = right-left;
    return pair;
}
/*void CutPair_free(CutPair *pair){
    // free(pair);
}*/

bool CutPair_in_between(CutPair *pair, uint64_t cut){
    return pair->left < cut && cut < pair->right;
}

size_t CutPair_cut_pair_mem(){
    return CutPair_mem()*2;
}
void CutPair_cut_pair(CutPair *src_pair, uint64_t cut, CutPair **dst_left, CutPair **dst_right){
    assert(CutPair_in_between(src_pair, cut));
    *dst_left = CutPair_init(src_pair->left, cut);
    *dst_right = CutPair_init(cut, src_pair->right);
}


typedef struct node{
    CutPair *self;
    struct node *left, *right;
} Node;

size_t Node_mem(){
    return sizeof(Node);
}
Node *Node_init(CutPair *self, Node *left, Node *right){
    Node *node = MemPool_alloc(sizeof(Node));
    node->self = self;
    node->left = left;
    node->right = right;
    return node;
}
/*void Node_free(Node *node){
    //free(node);
}
void Node_destroy(Node *node){
    CutPair_free(node->self);
    //free(node);
}*/

Node *Node_delete_node(Node *node){
    //CutPair_free(node->self);
    Node *aux = node->right;
    if(node->left != NULL){
        node->left->right = node->right;
    }
    if(node->right != NULL){
        node->right->left = node->left;
    }
    //Node_free(node);
    return aux;
}

size_t Node_insert_mem(){
    return Node_mem();
}
Node *Node_insert(Node *node, CutPair *new_val){
    Node *aux = Node_init(new_val, node->left, node);
    if(node->left != NULL){
        node->left->right = aux;
    }
    node->left = aux;
    return aux;
}
size_t Node_append_mem(){
    return Node_mem();
}
Node *Node_append(Node *node, CutPair *new_val){
    Node *aux = Node_init(new_val, node, node->right);
    if(node->right != NULL){
        node->right->left = aux;
    }
    node->right = aux;
    return aux;
}


typedef struct{
    Node *cuts_list_head;
    Node *widths_stack_head;
    Node *widths_stack_tail;
    //size_t len;
    //size_t size;
} CutsList;

size_t CutsList_mem(){
    return sizeof(CutsList) + Node_mem()*2 + CutPair_mem();
}
CutsList *CutsList_init(/*size_t elements_amount,*/ uint64_t left, uint64_t right){
    CutsList *list = MemPool_alloc(sizeof(CutsList));
    //list->size = elements_amount;
    CutPair *self = CutPair_init(left, right);

    list->cuts_list_head = Node_init(self, NULL, NULL);

    list->widths_stack_head = Node_init(self, NULL, NULL);
    list->widths_stack_tail = list->widths_stack_head;

    //list->len = 1;
    return list;
}
/*void CutsList_free(CutsList *list){
    Node *aux;
    Node *node = list->cuts_list_head;
    while(node != NULL){
        aux = node->right;
        //Node_destroy(node);
        node = aux;
    }
    node = list->widths_stack_head;
    while(node != NULL){
        aux = node->right;
        //Node_free(node);
        node = aux;
    }
    //free(list);
}*/

int64_t CutsList_biggest_diff(CutsList *list){
    return list->widths_stack_head->self->diff;
}

size_t CutsList_add_cut_mem(){
    return CutPair_cut_pair_mem() + Node_mem() + Node_append_mem()*2;
}
void CutsList_add_cut(CutsList *list, uint64_t cut){
    //assert(list->len < list->size);

    CutPair *old_pair;
    CutPair *dst_left;
    CutPair *dst_right;
    
    {
        bool cut_inserted = false;
        Node *cuts_node = list->cuts_list_head;
        while(cuts_node != NULL && !cut_inserted){
            if(CutPair_in_between(cuts_node->self, cut)){
                old_pair = cuts_node->self;
                CutPair_cut_pair(old_pair, cut, &dst_left, &dst_right);

                cuts_node->self = dst_left;
                cuts_node->right = Node_init(dst_right, cuts_node, cuts_node->right);

                cut_inserted = true;
            }
            cuts_node = cuts_node->right;
        }
        assert(cut_inserted);
    }

    {
        bool left_inserted = false;
        bool right_inserted = false;
        Node *widths_node = list->widths_stack_tail;
        while(widths_node != NULL && (!left_inserted || !right_inserted)){
            if(!left_inserted && dst_left->diff <= widths_node->self->diff){
                widths_node = Node_append(widths_node, dst_left);
                if(widths_node->right == NULL){
                    list->widths_stack_tail = widths_node;
                }
                left_inserted = true;
            }
            if(!right_inserted && dst_right->diff <= widths_node->self->diff){
                widths_node = Node_append(widths_node, dst_right);
                if(widths_node->right == NULL){
                    list->widths_stack_tail = widths_node;
                }
                right_inserted = true;
            }

            assert(widths_node != NULL);
            widths_node = widths_node->left;
        }
        assert(left_inserted);
        assert(right_inserted);
    }

    {
        bool width_removed = false;
        Node *widths_node = list->widths_stack_head;
        while(widths_node != NULL && !width_removed){
            if(widths_node->self == old_pair){
                widths_node = Node_delete_node(widths_node);
                if(widths_node->left == NULL){
                    list->widths_stack_head = widths_node;
                }
                if(widths_node->right == NULL){
                    list->widths_stack_tail = widths_node;
                }
                old_pair = NULL;
                width_removed = true;
            }

            assert(widths_node != NULL);
            widths_node = widths_node->right;
        }
        assert(width_removed);
    }
    //++list->len;
}


int main(){
    long w, h, n;
    scanf("%li %li %li", &w, &h, &n);
    //printf("n=%ld: %zu\n", n, 2*CutsList_mem() + n*CutsList_add_cut_mem());
    MemPool_init(2*CutsList_mem() + n*CutsList_add_cut_mem());

    CutsList *h_cuts = CutsList_init(/*n+1,*/ 0, h);
    CutsList *v_cuts = CutsList_init(/*n+1,*/ 0, w);

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

    //CutsList_free(h_cuts);
    //CutsList_free(v_cuts);

    MemPool_free();

    return 0;
}