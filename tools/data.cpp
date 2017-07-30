#include <bits/stdc++.h>
#include <windows.h>
using namespace std;
inline void
get_filename(char *prefix, char *filename, int count){

    char index[50];
    itoa(count, index, 10);
    strcpy(filename, prefix);
    int L = strlen(filename);

    int i, j;
    for(j = 0, i = L; j < strlen(index); j++, i++){
        filename[i] = index[j];
    }
    filename[i] = '\0';
    strcat(filename, ".txt");
}

int main(){

    char prefix[256];
    int range_left;
    int range_right;
    int num;
    int count;

    printf("Input output prefix filename : ");
    while(scanf("%s", prefix) != EOF){

        printf("Input left_range, right_range and number : ");
        scanf("%d %d %d", &range_left, &range_right, &num);
        printf("File count:");
        scanf("%d", &count);

        for(int j = 1; j <= count; j++){

            set<int> Set;
            char filename[256];
            get_filename(prefix, filename, j);
            printf("save in %s...\n", filename);

            freopen(filename, "w", stdout);
            Sleep(1000);
            time_t t = time(NULL);
            int temp_num = num;
            srand(t);

            while(temp_num){
                int val = 0;
                for(int i = 0; i < 8; i++){
                    val = val * 10 + (rand() % 10);
                }
                val = val % (range_right - range_left + 1) + range_left;
                if(Set.count(val) == 0){
                    Set.insert(val);
                    printf("%d\n", val);
                    temp_num--;
                }
            }
            fclose(stdout);
            freopen("CON", "w", stdout);
        }
        printf("Input output prefix filename : ");
    }
    return 0;
}
