#include "src/bplus_tree.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

static double
calc_timer(clock_t start, clock_t end){
    return (double)(end - start) / CLOCKS_PER_SEC;
}

static void
bplus_tree_insert_test(){

    char filename[128];
    int i, level, times;
    clock_t start, end;
    bplus_tree_t *bplus_tree;

    printf("Input tree's level : ");
    scanf("%d", &level);
    printf("Input test filename : ");

    while(scanf("%s", filename) != EOF){

        printf("Input test times : ");
        scanf("%d", &times);

        for(i = 0; i < times; i++){
            bplus_tree = new_bplus_tree(level);
            freopen(filename, "r", stdin);
            int value;
            start = clock();
            while(scanf("%d", &value) != EOF){
                bplus_tree_insert(bplus_tree, value);
            }
            end = clock();
            int left = bplus_tree_scan(bplus_tree);
            printf("Cost time : %f seconds. (times : %d/%d, count : %d)\n",
                   calc_timer(start, end),
                   i + 1, times,
                   left);
            fclose(stdin);
            freopen("CON", "r", stdin);
            bplus_tree_free(bplus_tree);
        }
        printf("Input test filename : ");
    }
}

static void
bplus_tree_delete_test(){

    char filename[128];
    int i, level, times, count;
    clock_t start, end;
    bplus_tree_t *bplus_tree;

    printf("Input tree's level : ");
    scanf("%d", &level);
    printf("Input test filename : ");

    while(scanf("%s", filename) != EOF){

        printf("Input delete times : ");
        scanf("%d", &count);
        printf("Input test times : ");
        scanf("%d", &times);

        for(i = 0; i < times; i++){
            //build tree
            bplus_tree = new_bplus_tree(level);
            freopen(filename, "r", stdin);
            int j, value;
            while(scanf("%d", &value) != EOF){
                bplus_tree_insert(bplus_tree, value);
            }
            fclose(stdin);

            freopen(filename, "r", stdin);
            start = clock();
            for(j = 0; j < count; j++){
                scanf("%d", &value);
                //printf("%d %d\n", j, value);
                bplus_tree_delete(bplus_tree, value);
            }
            end = clock();
            int left = bplus_tree_scan(bplus_tree);
            printf("Cost time : %f seconds.(times: %d/%d, count : %d)\n",
                   calc_timer(start, end),
                   i + 1, times,
                   left);
            fclose(stdin);
            bplus_tree_free(bplus_tree);
        }
        freopen("CON", "r", stdin);
        printf("Input test filename : ");
    }
}

static void
bplus_tree_scan_test(){

    char filename[128];
    int i, level, times, count;
    clock_t start, end;
    bplus_tree_t *bplus_tree;

    printf("Input tree's level : ");
    scanf("%d", &level);
    printf("Input test filename : ");

    while(scanf("%s", filename) != EOF){

        printf("Input scan times : ");
        scanf("%d", &count);
        printf("Input test times : ");
        scanf("%d", &times);

        bplus_tree = new_bplus_tree(level);
        freopen(filename, "r", stdin);
        int value;
        while(scanf("%d", &value) != EOF){
            bplus_tree_insert(bplus_tree, value);
        }
        fclose(stdin);
        freopen("CON", "r", stdin);
        for(i = 0; i < times; i++){
            //build tree
            int j, left;
            start = clock();
            for(j = 0; j < count; j++){
                left = bplus_tree_scan(bplus_tree);
            }
            end = clock();
            printf("Cost time : %f seconds.(times : %d/%d, count : %d)\n",
                   calc_timer(start, end),
                   i + 1, times,
                   left);
        }
        bplus_tree_free(bplus_tree);
        printf("Input test filename : ");
    }
}

static void
bplus_tree_search_test(){

    char filename[128];
    int i, level, times, count;
    clock_t start, end;
    bplus_tree_t *bplus_tree;

    printf("Input tree's level : ");
    scanf("%d", &level);
    printf("Input test filename : ");

    while(scanf("%s", filename) != EOF){

        printf("Input search times : ");
        scanf("%d", &count);
        printf("Input test times : ");
        scanf("%d", &times);

        for(i = 0; i < times; i++){
            //build tree
            bplus_tree = new_bplus_tree(level);
            freopen(filename, "r", stdin);
            int j, value;
            while(scanf("%d", &value) != EOF){
                bplus_tree_insert(bplus_tree, value);
            }
            fclose(stdin);

            freopen(filename, "r", stdin);
            start = clock();
            for(j = 0; j < count; j++){
                scanf("%d", &value);
                //printf("%d %d\n", j, value);
                bplus_tree_search(bplus_tree, value);
            }
            end = clock();
            int left = bplus_tree_scan(bplus_tree);
            printf("Cost time : %f seconds.(times : %d/%d, count : %d)\n",
                   calc_timer(start, end),
                   i + 1, times,
                   left);
            fclose(stdin);
            bplus_tree_free(bplus_tree);
        }
        freopen("CON", "r", stdin);
        printf("Input test filename : ");
    }
}

inline void
auto_test(){
    int opt;
    while(scanf("%d", &opt) != EOF){
        //insert
        if(opt == 1){
            bplus_tree_insert_test();
        }
        //delete
        else if(opt == 2){
            bplus_tree_delete_test();
        }
        //search
        else if(opt == 3){
            bplus_tree_search_test();
        }
        //scan
        else if(opt == 4){
            bplus_tree_scan_test();
        }
    }
}

inline void
hand_test(){

    bplus_tree_t *bplus_tree = new_bplus_tree(5);
    int opt, value;

    while(scanf("%d", &value) != EOF){
        bplus_tree_insert(bplus_tree, value);
    }

    while(scanf("%d", &opt) != EOF){
        //print
        if(opt == 0){
            bplus_tree_print(bplus_tree);
            puts("");
        }
        //insert
        else if(opt == 1){
            scanf("%d", &value);
            bplus_tree_insert(bplus_tree, value);
        }
        //delete
        else if(opt == 2){
            scanf("%d", &value);
            bplus_tree_delete(bplus_tree, value);
        }
        //search
        else if(opt == 3){
            scanf("%d", &value);
            bplus_node_t *bplus_node = bplus_tree_search(bplus_tree, value);
            if(bplus_node != NULL)
                printf("Found!\n");
            else
                printf("Null!\n");
        }
        //scan
        else if(opt == 4){
            int count = bplus_tree_scan(bplus_tree);
            printf("Count : %d\n", count);
        }
        //leaf
        else if(opt == 5){
            scanf("%d", &value);
            bplus_leaf_print(bplus_tree, value);
        }
    }
}

int main(){
    //auto_test();
    hand_test();
    return 0;
}
