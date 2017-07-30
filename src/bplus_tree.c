#include <stdio.h>
#include <malloc.h>
#include "bplus_tree.h"

//list option
bplus_node_t *list_header(bplus_node_t *bplus_node){
    while(bplus_node -> prev != NULL)
        bplus_node = bplus_node -> prev;
    return bplus_node;
}

bplus_node_t *list_index(bplus_node_t *bplus_node, int index){
    int cnt = 0;
    while(cnt < index){
        bplus_node = bplus_node -> next;
        cnt++;
    }
    return bplus_node;
}

inline void
list_print(bplus_node_t *header){

}

//new tree
inline bplus_tree_t*
new_bplus_tree(int level){

    bplus_tree_t* bplus_tree = (bplus_tree_t *)malloc(sizeof(bplus_tree_t));
    bplus_tree -> root = NULL;
    bplus_tree -> level = level;
    bplus_tree -> min_level = (level % 2 == 0) ? (level / 2) : (level / 2 + 1);
    return bplus_tree;

}

//new node
inline bplus_node_t*
new_bplus_node(int value, node_type_t type){

    bplus_node_t* bplus_node = (bplus_node_t *)malloc(sizeof(bplus_node_t));
    bplus_node -> value = value;
    bplus_node -> type = type;
    bplus_node -> right_count =
    bplus_node -> left_count = 0;
    bplus_node -> prev =
    bplus_node -> next =
    bplus_node -> left_link =
    bplus_node -> right_link =
    bplus_node -> parent =
    bplus_node -> child = NULL;
    return bplus_node;

}

//tree init
inline void
bplus_tree_init(bplus_tree_t *bplus_tree, int level){

    bplus_tree -> root = NULL;
    bplus_tree -> level = level;

}

inline bplus_node_t*
bplus_tree_search_lower_leaf(bplus_node_t *current_node, int value){

    //node type is leaf
    if(current_node -> type == leaf){
        while(current_node -> next != NULL){
            if(current_node -> value == value)
                return NULL;
            if(current_node -> next -> value > value){
                return current_node;
            }
            else{
                current_node = current_node -> next;
            }
        }
        if(current_node -> value == value)
            return NULL;
        else
            return current_node;
    }
    //node type is node
    else{
        while(current_node -> next != NULL){
            if(current_node -> next -> value > value){
                return bplus_tree_search_lower_leaf(current_node -> child, value);
            }
            else{
                current_node = current_node -> next;
            }
        }
        //larger than all of the value
        return bplus_tree_search_lower_leaf(current_node -> child, value);
    }
}

inline bplus_node_t*
bplus_tree_search_equal_leaf(bplus_node_t *current_node, int value){

    if(current_node == NULL)
        return NULL;
    if(current_node -> type == leaf){
        while(current_node != NULL){
            if(current_node -> value < value)
                current_node = current_node -> next;
            else if(current_node -> value == value)
                return current_node;
            else if(current_node -> value > value)
                break;
        }
        return NULL;
    }
    else{
        while(current_node -> next != NULL){
            if(current_node -> next -> value > value)
                return bplus_tree_search_equal_leaf(current_node -> child, value);
            else
                current_node = current_node -> next;
        }
        return bplus_tree_search_equal_leaf(current_node -> child, value);
    }
}

inline bplus_node_t*
bplus_tree_search_upper_leaf(bplus_node_t *current_node, int value){
    return NULL;
}

inline bplus_node_t*
bplus_tree_search(bplus_tree_t *bplus_tree, int value){
    if(bplus_tree == NULL || bplus_tree -> root == NULL)
        return NULL;
    else
        return bplus_tree_search_equal_leaf(bplus_tree -> root, value);
}

//delete option
//1.left adjust
//2.right adjust
//3.merge
inline void
bplus_tree_delete_adjust(bplus_tree_t *bplus_tree, bplus_node_t *current_node){

    //current_node must be leftest
    while(current_node -> prev != NULL){
        current_node = current_node -> prev;
    }

    //if need not to adjust
    if(current_node -> right_count >= bplus_tree -> min_level){

    }
    else{
        bplus_node_t *header_node = current_node;
        bplus_node_t *tail_node = current_node;
        while(current_node != NULL){
            tail_node = current_node;
            current_node = current_node -> next;
        }
        bplus_node_t *tail_left_node = header_node -> left_link;
        bplus_node_t *header_right_node = tail_node -> right_link;

        //if left can
        if(tail_left_node != NULL &&
           tail_left_node -> left_count > bplus_tree -> min_level){
            ///////////////////////////////////////////////
            // temp_node -> tail_left_node | header_node //
            // temp_node | tail_left_node -> header_node //
            ///////////////////////////////////////////////
            bplus_node_t *temp_node = tail_left_node -> prev;
            temp_node -> next = NULL;
            temp_node -> right_link = tail_left_node;
            //prev next
            tail_left_node -> prev = NULL;
            tail_left_node -> next = header_node;
            //link
            tail_left_node -> left_link = temp_node;
            tail_left_node -> right_link = NULL;
            //parent
            tail_left_node -> parent = header_node -> parent;
            tail_left_node -> parent -> child = tail_left_node;
            //count
            tail_left_node -> right_count = header_node -> right_count + 1;
            tail_left_node -> left_count = 1;
            //
            header_node -> parent = NULL;
            header_node -> prev = tail_left_node;
            header_node -> left_link = NULL;
            //adjust min value
            bplus_tree_delete_update(bplus_tree, tail_left_node -> parent, tail_left_node -> value);
            //count
            while(temp_node != NULL){
                temp_node -> right_count--;
                temp_node = temp_node -> prev;
            }
            while(header_node != NULL){
                header_node -> left_count++;
                header_node = header_node -> next;
            }
        }
        //if right can
        else if(header_right_node != NULL &&
            header_right_node -> right_count > bplus_tree -> min_level){
            ////////////////////////////////////////////////
            // tail_node | header_right_node -> temp_node //
            // tail_node -> header_right_node | temp_node //
            ////////////////////////////////////////////////
            bplus_node_t *temp_node = header_right_node -> next;
            temp_node -> prev = NULL;
            temp_node -> left_link = header_right_node;
            temp_node -> parent = header_right_node -> parent;
            temp_node -> parent -> child = temp_node;
            //next prev
            header_right_node -> next = NULL;
            header_right_node -> prev = tail_node;
            //link
            header_right_node -> right_link = temp_node;
            header_right_node -> left_link = NULL;
            //count
            header_right_node -> left_count = tail_node -> left_count + 1;
            header_right_node -> right_count = 1;
            //parent
            header_right_node -> parent = NULL;
            tail_node -> next = header_right_node;
            tail_node -> right_link = NULL;
            bplus_tree_delete_update(bplus_tree, temp_node -> parent, temp_node -> value);
            while(tail_node != NULL){
                tail_node -> right_count++;
                tail_node = tail_node -> prev;
            }
            while(temp_node != NULL){
                temp_node -> left_count--;
                temp_node = temp_node -> next;
            }
        }
        //else merge
        else if(tail_left_node != NULL || header_right_node != NULL){
            //printf("[merge...\n");
            //bplus_node_print(header_node);
            //bplus_node_print(header_right_node);
            bplus_node_t *mark_header_node;
            bplus_node_t *mark_tail_node;
            bplus_node_t *header_left_node;
            if(tail_left_node != NULL){
                bplus_node_t *temp_node = tail_left_node;
                header_left_node = temp_node;
                while(temp_node != NULL){
                    header_left_node = temp_node;
                    temp_node = temp_node -> prev;
                }
            }
            //printf("[left right merge...\n");
            //left merge
            if(tail_left_node != NULL &&
               //header_left_node -> parent != NULL &&
               header_left_node -> parent -> next == header_node -> parent){
                //printf("[merge left.\n");
                ///////////////////////////////////
                // tail_left_node | header_node  //
                // tail_left_node -> header_node //
                ///////////////////////////////////
                //cut link
                tail_left_node -> right_link = NULL;
                header_node -> left_link = NULL;
                //parent (prev next link)
                header_left_node -> parent -> next = header_node -> parent -> next;
                if(header_node -> parent -> next != NULL){
                    header_node -> parent -> next -> prev = header_left_node -> parent;
                }
                header_left_node -> parent -> right_link = header_node -> parent -> right_link;
                if(header_node -> parent -> right_link != NULL){
                    header_node -> parent -> right_link -> left_link =
                    header_left_node -> parent;
                }
                free(header_node -> parent);
                header_node -> parent = NULL;
                //merge
                tail_left_node -> next = header_node;
                header_node -> prev = tail_left_node;
                //mark
                mark_header_node = header_left_node;
                //printf("merge left.]\n");
            }
            else if(header_right_node != NULL &&
                    //header_node -> parent != NULL &&
                    header_node -> parent -> next == header_right_node -> parent){
                //printf("[merge right.\n");
                ////////////////////////////////////
                // tail_node | header_right_node  //
                // tail_node -> header_right_node //
                ////////////////////////////////////
                //cut link
                header_right_node -> left_link = NULL;
                tail_node -> right_link = NULL;
                //parent
                header_node -> parent -> next = header_right_node -> parent -> next;
                if(header_right_node -> parent -> next != NULL){
                    header_right_node -> parent -> next -> prev = header_node -> parent;
                }
                header_node -> parent -> right_link = header_right_node -> parent -> right_link;
                if(header_right_node -> parent -> right_link != NULL){
                    header_right_node -> parent -> right_link -> left_link = header_node -> parent;
                }
                free(header_right_node -> parent);
                header_right_node -> parent = NULL;
                //merge
                tail_node -> next = header_right_node;
                header_right_node -> prev = tail_node;
                //mark
                mark_header_node = header_node;
                //printf("merge right.]\n");
            }
            //else{
            //    printf("error\n");
            //}
            //printf("left right merge...]\n");
            //count
            bplus_node_t *mark_parent_node = mark_header_node -> parent;
            bplus_node_t *temp_parent_node = mark_parent_node;
            while(mark_parent_node != NULL){
                mark_parent_node -> right_count--;
                mark_parent_node = mark_parent_node -> prev;
            }
            mark_parent_node = mark_header_node -> parent -> next;
            while(mark_parent_node != NULL){
                mark_parent_node -> left_count--;
                mark_parent_node = mark_parent_node -> next;
            }
            while(mark_header_node != NULL){
                mark_header_node -> left_count =
                (mark_header_node -> prev == NULL) ? 1 : mark_header_node -> prev -> left_count + 1;
                mark_tail_node = mark_header_node;
                mark_header_node = mark_header_node -> next;
            }
            while(mark_tail_node != NULL){
                mark_tail_node -> right_count =
                (mark_tail_node -> next == NULL) ? 1 : mark_tail_node -> next -> right_count + 1;
                mark_tail_node = mark_tail_node -> prev;
            }
            //bplus_node_print(temp_parent_node);
            bplus_tree_delete_adjust(bplus_tree, temp_parent_node);
            //printf("merge...]\n");
        }
        //else only one node delete
        else{
            //printf("error : can no t merge.\n");
        }
    }
}

inline void
bplus_tree_delete_update(bplus_tree_t *bplus_tree, bplus_node_t *bplus_node, int value){
    while(bplus_node != NULL){
        bplus_node -> value = value;
        bplus_node = bplus_node -> parent;
    }
}

inline void
bplus_tree_delete_reset_root(bplus_tree_t *bplus_tree){

    bplus_node_t *temp_node = bplus_tree -> root;
    bplus_node_t *parent_node;

    while(temp_node != NULL){
        if(temp_node -> left_count == 1 && temp_node -> right_count == 1){
            bplus_tree -> root = temp_node -> child;
            parent_node = temp_node;
            if(temp_node -> child != NULL)
                temp_node -> child -> parent = NULL;
            temp_node = temp_node -> child;
            free(parent_node);
        }
        else break;
    }
}

inline void
bplus_tree_delete(bplus_tree_t *bplus_tree, int value){

    //if tree is null
    if(bplus_tree == NULL)
        return;

    //find leaf
    bplus_node_t *del_node = bplus_tree_search_equal_leaf(bplus_tree -> root, value);
    bplus_node_t *header_node;
    //if null
    if(del_node == NULL)
        return;
    //update left value
    if(del_node -> prev == NULL){
        //is not root
        if(del_node -> parent != NULL){
            bplus_node_t *parent = del_node -> parent;
            bplus_node_t *next_node = del_node -> next;
            //update link
            if(del_node -> left_link != NULL){
                del_node -> left_link -> right_link = next_node;
                next_node -> left_link = del_node -> left_link;
            }
            //mark header node
            header_node = next_node;
            parent -> child = next_node;
            next_node -> parent = parent;
            next_node -> prev = NULL;
            bplus_tree_delete_update(bplus_tree, parent, next_node -> value);
            free(del_node);
        }
        //is root
        else{
            bplus_tree -> root = del_node -> next;
            //if(del_node -> next != NULL)
            del_node -> next -> prev = NULL;
            //mark header node
            header_node = del_node -> next;
            free(del_node);
        }
        //count
    }
    else{
        bplus_node_t *prev_node = del_node -> prev;
        prev_node -> next = del_node -> next;
        //if is not tail
        if(del_node -> next != NULL){
            del_node -> next -> prev = prev_node;
        }
        //if is tail
        else{
            if(del_node -> right_link != NULL){
                prev_node -> right_link = del_node -> right_link;
                del_node -> right_link -> left_link = prev_node;
            }
        }
        //update right count
        while(prev_node != NULL){
            prev_node -> right_count--;
            header_node = prev_node;
            prev_node = prev_node -> prev;
        }
        free(del_node);
    }
    //update left count
    bplus_node_t *temp_node = header_node;
    while(temp_node != NULL){
        temp_node -> left_count =
        (temp_node -> prev == NULL) ? 1 : temp_node -> prev -> left_count + 1;
        temp_node = temp_node -> next;
    }
    //printf("%d (%d %d)\n", header_node -> value,
    //       header_node -> left_count, header_node -> right_count);
    //printf("root : %d\n", bplus_tree -> root -> value);
    //bplus_node_print(header_node);
    bplus_tree_delete_adjust(bplus_tree, header_node);
    //reset root
    bplus_tree_delete_reset_root(bplus_tree);
}

//insert option
inline void
bplus_tree_insert_adjust(bplus_tree_t *bplus_tree, bplus_node_t *current_node){

    //if need not to adjust
    if(current_node -> right_count <= bplus_tree -> level){
        return;
    }
    else{
        //top floor
        if(current_node -> parent == NULL){

            bplus_node_t *mid_node = list_index(current_node, current_node -> right_count / 2);
            int left_num  = current_node -> right_count / 2;
            int right_num = (current_node -> right_count) - (current_node -> right_count / 2);
            bplus_node_t *parent1 = new_bplus_node(current_node -> value, node);
            bplus_node_t *parent2 = new_bplus_node(mid_node -> value, node);

            current_node -> parent = parent1;
            parent1 -> child = current_node;
            parent1 -> right_count = 2;
            parent1 -> left_count = 1;

            mid_node -> parent = parent2;
            parent2 -> child = mid_node;
            parent2 -> right_count = 1;
            parent2 -> left_count = 2;

            parent1 -> next = parent2;
            parent2 -> prev = parent1;

            //cut
            mid_node -> prev -> right_link = mid_node;
            mid_node -> left_link = mid_node -> prev;
            mid_node -> prev -> next = NULL;
            mid_node -> prev = NULL;

            //right count
            while(current_node != NULL){
                current_node -> right_count -= right_num;
                current_node = current_node -> next;
            }
            //left count
            while(mid_node != NULL){
                mid_node -> left_count -= left_num;
                mid_node = mid_node -> next;
            }
            bplus_tree -> root = parent1;
        }
        else{

            bplus_node_t *mid_node = list_index(current_node, current_node -> right_count / 2);
            int left_num  = current_node -> right_count / 2;
            int right_num = (current_node -> right_count) - (current_node -> right_count / 2);

            bplus_node_t *parent = current_node -> parent;
            //printf("[%d] (%d, %d)\n", parent -> value, parent -> left_count, parent -> right_count);
            bplus_node_t *new_node = new_bplus_node(mid_node -> value, node);
            new_node -> value = mid_node -> value;
            new_node -> prev = parent;
            new_node -> next = parent -> next;

            if(parent -> next != NULL)
                parent -> next -> prev = new_node;  //error
            parent -> next = new_node;

            new_node -> right_link = parent -> right_link;
            if(parent -> right_link != NULL){
                parent -> right_link -> left_link = new_node;
                parent -> right_link = NULL;
            }

            //cut
            mid_node -> prev -> right_link = mid_node;
            mid_node -> left_link = mid_node -> prev;
            mid_node -> prev -> next = NULL;
            mid_node -> prev = NULL;

            //link
            new_node -> child = mid_node;
            mid_node -> parent = new_node;

            //count
            new_node -> right_count = parent -> right_count;
            new_node -> left_count = parent -> left_count;
            bplus_node_t *temp_node = parent;
            bplus_node_t *temp2_node = new_node;

            while(temp_node != NULL){
                temp_node -> right_count++;
                parent = temp_node;
                temp_node = temp_node -> prev;
            }
            while(temp2_node != NULL){
                temp2_node -> left_count++;
                temp2_node = temp2_node -> next;
            }
            while(current_node != NULL){
                current_node -> right_count -= right_num;
                current_node = current_node -> next;
            }
            while(mid_node != NULL){
                mid_node -> left_count -= left_num;
                mid_node = mid_node -> next;
            }

            //continue to adjust
            bplus_tree_insert_adjust(bplus_tree, parent);
        }
    }
}

inline void
bplus_tree_insert_update(bplus_tree_t *bplus_tree, bplus_node_t *bplus_node, int value){
    while(bplus_node != NULL){
        bplus_node -> value = value;
        bplus_node = bplus_node -> parent;
    }
}

inline void
bplus_tree_insert(bplus_tree_t *bplus_tree, int value){

    //tree is null
    if(bplus_tree == NULL)
        return;

    bplus_node_t *root = bplus_tree -> root;
    bplus_node_t *new_node = new_bplus_node(value, leaf);

    //first insert
    if(root == NULL){
        bplus_tree -> root = new_node;
    }
    else{
        bplus_node_t *location = bplus_tree_search_lower_leaf(root, value);
        //value has existed
        if(location == NULL)
            return;
        //is not first
        if(location -> value < value){
            new_node -> next = location -> next;
            new_node -> prev = location;
            if(location -> next != NULL){
                location -> next -> prev = new_node;
            }
            if(location -> right_link != NULL){
                new_node -> right_link = location -> right_link;
                location -> right_link -> left_link = new_node;
                location -> right_link = NULL;
            }
            location -> next = new_node;
        }
        //is first
        else{
            new_node -> next = location;
            location -> prev = new_node;
            new_node -> parent = location -> parent;
            location -> parent = NULL;
            //link
            if(location -> left_link != NULL){
                new_node -> left_link = location -> left_link;
                location -> left_link -> right_link = new_node;
            }
            if(new_node -> parent != NULL){
                new_node -> parent -> child = new_node;
            }
            //only one floor
            else{
                bplus_tree -> root = new_node;
            }
            bplus_tree_insert_update(bplus_tree, new_node, value);
        }
    }
    //adjust leaf floor
    bplus_node_t *header_node = new_node;
    bplus_node_t *tail_node = new_node;
    bplus_node_t *temp_node = new_node;
    //get header
    while(temp_node != NULL){
        header_node = temp_node;
        temp_node = temp_node -> prev;
    }
    //get tail
    temp_node = new_node;
    while(temp_node != NULL){
        tail_node = temp_node;
        temp_node = temp_node -> next;
    }
    //left -> right update left_count
    temp_node = header_node;
    while(temp_node != NULL){
        temp_node -> left_count =
        (temp_node -> prev == NULL) ? 1 : temp_node -> prev -> left_count + 1;
        temp_node = temp_node -> next;
    }
    temp_node = tail_node;
    while(temp_node != NULL){
        temp_node -> right_count =
        (temp_node -> next == NULL) ? 1 : temp_node -> next -> right_count + 1;
        temp_node = temp_node -> prev;
    }
    bplus_tree_insert_adjust(bplus_tree, header_node);
}

//free
inline void
bplus_tree_dfs_free(bplus_node_t *current_node){
    bplus_node_t *temp_node = current_node;
    while(current_node != NULL){
        if(current_node -> child != NULL)
            bplus_tree_dfs_free(current_node -> child);
        temp_node = current_node;
        current_node = current_node -> next;
        free(temp_node);
    }
}

inline void
bplus_tree_free(bplus_tree_t *bplus_tree){

    if(bplus_tree == NULL || bplus_tree -> root == NULL)
        return;
    else
        bplus_tree_dfs_free(bplus_tree -> root);
}

//print
inline void
bplus_tree_dfs_print(bplus_node_t *current_node){

    printf("[");
    while(current_node != NULL){
        printf("%d (%d | %d) ",
               current_node -> value, current_node -> left_count, current_node -> right_count);
        if(current_node -> child != NULL)
            bplus_tree_dfs_print(current_node -> child);
        current_node = current_node -> next;
    }
   printf("]");

}

inline void
bplus_tree_print(bplus_tree_t *bplus_tree){

    if(bplus_tree == NULL || bplus_tree -> root == NULL)
        return;
    else
        bplus_tree_dfs_print(bplus_tree -> root);
}

inline void
bplus_leaf_print(bplus_tree_t *bplus_tree, int value){

    if(bplus_tree == NULL || bplus_tree -> root == NULL)
        return;

    bplus_node_t *bplus_leaf = bplus_tree_search_equal_leaf(bplus_tree -> root, value);
    if(bplus_leaf == NULL){
        printf("Can not find leaf!\n");
    }
    else{
        bplus_node_print(bplus_leaf);
    }
}

inline void
bplus_node_print(bplus_node_t *bplus_node){

    printf("-------------\n");
    if(bplus_node == NULL){
        printf("Null node.\n");
    }
    else{
        printf("[value]  : %d\n", bplus_node -> value);
        //next
        printf("[next]   : ");
        if(bplus_node -> next != NULL)
            printf("%d\n", bplus_node -> next -> value);
        else
            printf("null\n");
        //prev
        printf("[prev]   : ");
        if(bplus_node -> prev != NULL)
            printf("%d\n", bplus_node -> prev -> value);
        else
            printf("null\n");
        //parent
        printf("[parent] : ");
        if(bplus_node -> parent != NULL)
            printf("%d\n", bplus_node -> parent -> value);
        else
            printf("null\n");
        //left link
        printf("[l_link] : ");
        if(bplus_node -> left_link != NULL)
            printf("%d\n", bplus_node -> left_link -> value);
        else
            printf("null\n");
        //right link
        printf("[r_link] : ");
        if(bplus_node -> right_link != NULL)
            printf("%d\n", bplus_node -> right_link -> value);
        else
            printf("null\n");
        printf("[l_count]: %d\n", bplus_node -> left_count);
        printf("[r_count]: %d\n", bplus_node -> right_count);
    }
    printf("-------------\n");
}

//scan
inline int
bplus_tree_scan(bplus_tree_t *bplus_tree){

    bplus_node_t *bplus_node = bplus_tree -> root;
    int count = 0;

    if(bplus_node == NULL || bplus_tree -> root == NULL)
        return count;

    while(bplus_node -> child != NULL){
        bplus_node = bplus_node -> child;
    }
    while(1){
        count++;
        if(bplus_node -> next != NULL){
            bplus_node = bplus_node -> next;
        }
        else if(bplus_node -> right_link != NULL){
            bplus_node = bplus_node -> right_link;
        }
        else break;
    }
    return count;
}
