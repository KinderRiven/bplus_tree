#ifndef BPLUS_TREE_H_INCLUDED
#define BPLUS_TREE_H_INCLUDED

#ifndef NULL
    #define NULL 0
#endif /// NULL

typedef enum{
    leaf = 0,
    node = 1,
    root = 2
} node_type_t;

typedef struct bplus_node_t{

    struct bplus_node_t *parent;        //node's parent
    struct bplus_node_t *prev;          //node's left
    struct bplus_node_t *next;          //node's right
    struct bplus_node_t *child;         //node's child
    struct bplus_node_t *right_link;    //right  brother linker
    struct bplus_node_t *left_link;     //left   brother linker

    int value;                          //node's value
    int right_count;                    //node's right count
    int left_count;                     //node's left count
    node_type_t type;                   //node's type

} bplus_node_t;

typedef struct bplus_tree_t{

    bplus_node_t *root;                 //b+tree's gen
    int level;                          //b+tree's level
    int min_level;                      //b+tree's min level

} bplus_tree_t;

///list
bplus_node_t* list_header(bplus_node_t *bplus_node);
bplus_node_t* list_index(bplus_node_t *bplus_node, int index);
inline void list_print(bplus_node_t *header);

///new
//new tree
inline bplus_tree_t* new_bplus_tree(int level);

//new node
inline bplus_node_t* new_bplus_node(int value, node_type_t type);

///option
//init
inline void bplus_tree_init(bplus_tree_t *bplus_tree, int level);

//insert adjust
inline void bplus_tree_insert_adjust(bplus_tree_t *bplus_tree, bplus_node_t *current_node);

//delete adjust
inline void bplus_tree_delete_adjust(bplus_tree_t *bplus_tree, bplus_node_t *current_node);

//update left min value
inline void bplus_tree_delete_update(bplus_tree_t *bplus_tree, bplus_node_t *bplus_node, int value);

//update left min value
inline void bplus_tree_insert_update(bplus_tree_t *bplus_tree, bplus_node_t *bplus_node, int value);

//delete and reset
inline void bplus_tree_delete_reset_root(bplus_tree_t *bplus_tree);

//search a lower leaf
inline bplus_node_t* bplus_tree_search_lower_leaf(bplus_node_t *current_node, int value);

//search a equal leaf
inline bplus_node_t* bplus_tree_search_equal_leaf(bplus_node_t *current_node, int value);

//search a upper leaf
inline bplus_node_t* bplus_tree_search_upper_leaf(bplus_node_t *current_node, int value);

//dfs print
inline void bplus_tree_dfs_print(bplus_node_t *current_node);

//dfs free
inline void bplus_tree_dfs_free(bplus_node_t *current_node);

//node print
inline void bplus_node_print(bplus_node_t *bplus_node);

//leaf print
inline void bplus_leaf_print(bplus_tree_t *bplus_tree, int value);

///option
//insert into b+ tree
inline void bplus_tree_insert(bplus_tree_t *bplus_tree, int value);

//delete from b+ tree
inline void bplus_tree_delete(bplus_tree_t *bplus_tree, int value);

//search from b+ tree
inline bplus_node_t* bplus_tree_search(bplus_tree_t *bplus_tree, int value);

//scan
inline int bplus_tree_scan(bplus_tree_t *bplus_tree);

//print
inline void bplus_tree_print(bplus_tree_t *bplus_tree);

//free
inline void bplus_tree_free(bplus_tree_t *bplus_tree);

#endif // BPLUS_TREE_H_INCLUDED
