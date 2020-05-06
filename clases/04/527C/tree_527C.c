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


typedef struct tree_node{
    uint64_t value;
    struct tree_node *left, *right, *parent;
    bool red;
} TreeNode;

size_t TreeNode_init_mem(){
    return sizeof(TreeNode);
}
TreeNode *TreeNode_init(uint64_t value, TreeNode *left, TreeNode *right, TreeNode *parent){
    TreeNode *node = MemPool_alloc(sizeof(TreeNode));
    node->value = value;

    node->left = left;
    node->right = right;
    node->parent = parent;
    node->red = true;

    return node;
}

/*size_t TreeNode_three_mem(){
    return 3*TreeNode_init_mem();
}
TreeNode *TreeNode_three(uint64_t left, uint64_t middle, uint64_t right){
    TreeNode *middle = TreeNode_init(middle, NULL, NULL, NULL);

    TreeNode *left = TreeNode_init(left, NULL, NULL, middle);
    TreeNode *right = TreeNode_init(right, NULL, NULL, middle);

    middle->left = left;
    middle->right = right;

    return middle;
}*/

void TreeNode_rotate_left(TreeNode *node){
    assert(node != NULL);
    TreeNode *new = node->right;
    TreeNode *parent = node->parent;

    node->right = new->left;
    new->left = node;
    node->parent = new;

    if(node->right != NULL){
        node->right->parent = node;
    }

    if(parent != NULL){
        if(node == parent->left){
            parent->left = new;
        }
        else if(node == parent->right){
            parent->right = new;
        }
    }
    new->parent = parent;
}
void TreeNode_rotate_right(TreeNode *node){
    assert(node != NULL);
    TreeNode *new = node->left;
    TreeNode *parent = node->parent;

    node->left = new->right;
    new->right = node;
    node->parent = new;

    if(node->left != NULL){
        node->left->parent = node;
    }

    if(parent != NULL){
        if(node == parent->left){
            parent->left = new;
        }
        else if(node == parent->right){
            parent->right = new;
        }
    }
    new->parent = parent;
}

TreeNode *TreeNode_uncle(TreeNode *node){
    assert(node != NULL);
    TreeNode *aux = node->parent;
    if(aux == NULL || aux->parent == NULL){
        return NULL;
    }

    aux = aux->parent;
    return (aux->left == node->parent) ? aux->right : aux->left;
}

void TreeNode_repair(TreeNode *node){
    TreeNode *parent = node->parent;
    TreeNode *uncle;
    if(parent == NULL){
        node->red = false;
    }
    else if(parent->red == false){
        // pass
    }
    else if((uncle=TreeNode_uncle(node)) != NULL && uncle->red){
        parent->red = false;
        uncle->red = false;
        parent->parent->red = true;
        TreeNode_repair(parent->parent);
    }
    else{
        if(node == parent->right && parent == parent->parent->left){
            TreeNode_rotate_left(parent);
            node = node->left;
            parent = node->parent;
        }
        else if(node == parent->left && parent == parent->parent->right){
            TreeNode_rotate_right(parent);
            node = node->right;
            parent = node->parent;
        }

        parent->red = false;
        parent->parent->red = true;
        if(node == parent->left){
            TreeNode_rotate_right(parent->parent);
        }
        else{
            TreeNode_rotate_left(parent->parent);
        }
    }
}


typedef struct{
    TreeNode *middle;
    TreeNode *smaller, *largest;
} Tree;

/*size_t Tree_init_mem(){
    return sizeof(Tree) + TreeNode_three_mem();
}
Tree *Tree_init(uint64_t left, uint64_t middle, uint64_t right){
    Tree *tree = MemPool_alloc(sizeof(Tree));
    tree->middle = TreeNode_three(left, middle, right);
    tree->smaller = tree->left;
    tree->largest = tree->right;

    return tree;
}*/
size_t Tree_init_mem(){
    return sizeof(Tree);
}
Tree *Tree_init(){
    Tree *tree = MemPool_alloc(sizeof(Tree));
    tree->middle = NULL;
    tree->smaller = NULL;
    tree->largest = NULL;

    return tree;
}

size_t Tree_add_unsafe_mem(){
    return TreeNode_init_mem();
}
TreeNode *Tree_add_unsafe(Tree *tree, uint64_t value){
    TreeNode *node = tree->middle;
    bool only_left = true, only_right = true;
    bool inserted = false;
    if(tree->middle == NULL){
        tree->middle = node = TreeNode_init(value, NULL, NULL, NULL);
        inserted = true;
    }
    while(!inserted){
        if(value < node->value){
            only_right = false;
            if(node->left == NULL){
                node->left = TreeNode_init(value, NULL, NULL, node);
                inserted = true;
            }
            node = node->left;
        }
        else{
            only_left = false;
            if(node->right == NULL){
                node->right = TreeNode_init(value, NULL, NULL, node);
                inserted = true;
            }
            node = node->right;
        }
    }
    assert(inserted);

    if(only_left){
        tree->smaller = node;
    }
    if(only_right){
        tree->largest = node;
    }
    
    return node;
}
size_t Tree_add_mem(){
    return Tree_add_unsafe_mem();
}
TreeNode *Tree_add(Tree *tree, uint64_t value){
    TreeNode *node = Tree_add_unsafe(tree, value);
    TreeNode_repair(node);

    while(tree->middle->parent != NULL){
        tree->middle = tree->middle->parent;
    }

    return node;
}

bool Tree_delete_unsafe(Tree *tree, uint64_t value){
    assert(tree->middle != NULL);
    if(tree->middle->value == value){
        if(tree->middle == tree->smaller && tree->middle == tree->largest){
            tree->middle = tree->smaller = tree->largest = NULL;
            return true;
        }
        else if(tree->middle == tree->smaller){
            tree->middle = tree->smaller = tree->middle->right;
            tree->middle->parent = NULL;
            while(tree->smaller->left != NULL){
                tree->smaller = tree->smaller->left;
            }
        }
        else if(tree->middle == tree->largest){
            tree->middle = tree->largest = tree->middle->left;
            while(tree->largest->right != NULL){
                tree->largest = tree->largest->right;
            }
            tree->middle->parent = NULL;
        }
        else{
            uint64_t aux_val;
            if(tree->middle->left == tree->smaller){
                aux_val = tree->smaller->value;
                tree->middle = tree->middle->right;
                tree->middle->parent = NULL;
                Tree_add_unsafe(tree, aux_val);
            }
            else if(tree->middle->right == tree->largest){
                aux_val = tree->largest->value;
                tree->middle = tree->middle->left;
                tree->middle->parent = NULL;
                Tree_add_unsafe(tree, aux_val);
            }
            else{
                TreeNode *aux, *other_side;
                if(tree->middle->left != NULL){
                    other_side = tree->middle->right;
                    tree->middle = aux = tree->middle->left;
                    tree->middle->parent = NULL;
                    while(aux->right != NULL){
                        aux = aux->right;
                    }
                    aux->right = other_side;
                    other_side->parent = aux;
                }
                else{
                    other_side = tree->middle->left;
                    tree->middle = aux = tree->middle->right;
                    tree->middle->parent = NULL;
                    while(aux->left != NULL){
                        aux = aux->left;
                    }
                    aux->left = other_side;
                    other_side->parent = aux;
                }
            }
        }
        return true;
    }

    if(tree->largest->value == value){
        if(tree->largest->left != NULL){
            tree->largest->left->parent = tree->largest->parent;
            tree->largest = tree->largest->left;
            while(tree->largest->right != NULL){
                tree->largest = tree->largest->right;
            }
        }
        else{
            tree->largest = tree->largest->parent;
            tree->largest->right = NULL;
        }

        return true;
    }
    else if(tree->smaller->value == value){
        if(tree->smaller->right != NULL){
            tree->smaller->right->parent = tree->smaller->parent;
            tree->smaller = tree->smaller->right;
            while(tree->smaller->left != NULL){
                tree->smaller = tree->smaller->left;
            }
        }
        else{
            tree->smaller = tree->smaller->parent;
            tree->smaller->left = NULL;
        }
        
        return true;
    }

    TreeNode *node = tree->middle;
    TreeNode *parent = NULL;
    bool is_left_child = false;
    while(node != NULL){
        if(node->value == value){
            // destruir
            TreeNode *aux, *other_side;
            if(node->left != NULL){
                aux = node->left;
                other_side = node->right;
                if(is_left_child){
                    parent->left = aux;
                }
                else{
                    parent->right = aux;
                }
                if(aux != NULL){
                    aux->parent = parent;
                    while(aux->right != NULL){
                        aux = aux->right;
                    }
                    aux->right = other_side;
                }
                other_side->parent = aux;
            }
            else{
                aux = node->right;
                if(aux != NULL){
                    aux->parent = parent;
                }
                if(is_left_child){
                    parent->left = aux;
                }
                else{
                    parent->right = aux;
                }
            }
            
            return true;
        }
        else if(value < node->value){
            parent = node;
            node = node->left;
            is_left_child = true;
        }
        else{
            parent = node;
            node = node->right;
            is_left_child = false;
        }
    }
    return false;
}

TreeNode *Tree_find(Tree *tree, uint64_t value){
    TreeNode *node = tree->middle;

    while(node != NULL){
        if(node->value == value){
            return node;
        }
        else if(node->value < value){
            node = node->right;
        }
        else{
            node = node->left;
        }
    }

    return NULL;
}

bool Tree_find_limits(Tree *tree, uint64_t value, TreeNode **dst_left, TreeNode **dst_right){
    TreeNode *node = tree->middle;

    bool found = false;
    while(node != NULL){
        if(value > node->value){
            if(node->right == NULL){
                return found;
            }
            if(value < node->right->value){
                *dst_left = node;
                *dst_right = node->right;
                found = true;
            }
            node = node->right;
        }
        else if(value < node->value){
            if(node->left == NULL){
                return found;
            }
            if(value > node->left->value){
                *dst_left = node->left;
                *dst_right = node;
                found = true;
            }
            node = node->left;
        }
        else{
            return found;
        }
    }

    return found;
}



int main(){
    long w, h, n;
    scanf("%li %li %li", &w, &h, &n);
    //printf("n=%ld: %zu\n", n, 4*Tree_init_mem() + n*Tree_add_mem());
    MemPool_init(4*Tree_init_mem() + (6+3*n)*Tree_add_mem());

    Tree *h_cuts = Tree_init();
    Tree_add(h_cuts, 0);
    Tree_add(h_cuts, h);
    Tree *v_cuts = Tree_init();
    Tree_add(v_cuts, 0);
    Tree_add(v_cuts, w);

    Tree *h_width = Tree_init();
    Tree_add(h_width, h);
    Tree *v_width = Tree_init();
    Tree_add(v_width, w);

    char direction;
    uint64_t distance;
    TreeNode *left, *right;
    bool found;
    for(long i = 0; i < n; ++i){
        scanf(" %c %"SCNu64, &direction, &distance);

        if(direction == 'H'){
            found = Tree_find_limits(h_cuts, distance, &left, &right);
            assert(found);

            found = Tree_delete_unsafe(h_width, right->value - left->value);
            assert(found);

            Tree_add_unsafe(h_width, distance - left->value);
            Tree_add_unsafe(h_width, right->value - distance);
            
            Tree_add(h_cuts, distance);
        }
        else{
            found = Tree_find_limits(v_cuts, distance, &left, &right);
            assert(found);

            found = Tree_delete_unsafe(v_width, right->value - left->value);
            assert(found);

            Tree_add_unsafe(v_width, distance - left->value);
            Tree_add_unsafe(v_width, right->value - distance);
            
            Tree_add(v_cuts, distance);
        }
        printf("%"PRIu64"\n", h_width->largest->value*v_width->largest->value);
    }

    //CutsList_free(h_cuts);
    //CutsList_free(v_cuts);

    MemPool_free();

    return 0;
}
