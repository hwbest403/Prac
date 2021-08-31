#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

inline int mazeArrI(int i){
    return 2*i + 1;
}

inline int mazeArrJ(int j){
    return 2*j + 1;
}
int N, M;
int mazeSetArr[1000][1000];
char mazeArr[3000][3000];
int vis[1000][1000];

void search(int i, int j, int currNum){
    if(i >= N || j >= M || i < 0 || j < 0) return;
    if(vis[i][j]) return;
    vis[i][j] = 1;
    mazeSetArr[i][j] = currNum;
    if(mazeArr[mazeArrI(i)+1][mazeArrJ(j)] == ' '){
        search(i+1, j, currNum);
    }
    if(mazeArr[mazeArrI(i)][mazeArrJ(j)+1] == ' '){
        search(i, j+1, currNum);
    }
    if(mazeArr[mazeArrI(i)-1][mazeArrJ(j)] == ' '){
           search(i-1, j, currNum);
    }
    if(mazeArr[mazeArrI(i)][mazeArrJ(j)-1] == ' '){
        search(i, j-1, currNum);
    }
}

void dfs(){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            vis[i][j] = 0;
        }
    }
    
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            search(i, j, mazeSetArr[i][j]);
        }
    }
}

void printMaze(FILE*fp){
    for(int i = 0; i <= N*2; i++){
        for(int j = 0; j <= M*2; j++){
            fprintf(fp, "%c", mazeArr[i][j]);
            //printf("%c", mazeArr[i][j]);
        }
        fprintf(fp, "\n");
        //printf("\n");
    }
}

int main(int argc, const char * argv[]) {

    FILE* fp;
    fp = fopen("out.maz", "w");
    srand((unsigned int)time(NULL));
    
    scanf("%d %d", &N, &M);
    
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            mazeSetArr[i][j] = i*M + j;
        }
    }
    
    for(int i = 0; i <= N*2; i+=2){
        for(int j = 0; j < M*2; j+=2){
            mazeArr[i][j] = '+';
            mazeArr[i][j+1] = '-';
        }
        mazeArr[i][M*2] = '+';
    }
    
    for (int i = 1; i < N * 2; i += 2) {
        for (int j = 0; j < M * 2; j += 2) {
            mazeArr[i][j] = '|';
            mazeArr[i][j + 1] = ' ';
        }
        mazeArr[i][M * 2] = '|';
    }

    int random;
    int check = -1;

    for (int i = 0; i < N ; i++) {
        //수직벽 없애기
        for (int j = 0; j < M-1 ; j++) {
            random = rand() % 2;
            if (mazeSetArr[i][j] != mazeSetArr[i][j + 1]) {
                if (random == 0) continue;
                else {
                    mazeArr[2 * i + 1][2 * j + 2] = ' ';
                    dfs();
                }
            }
        }
        if (i != N - 1) {
            //적어도 하나의 하단벽 없애기
            for (int j = 0; j < M; j++) {
                if (mazeSetArr[i][j] != mazeSetArr[i][j+1]) { 
                    random = rand() % (j - check) + check + 1;
                    mazeArr[2 * i + 2][2 * random + 1] = ' ';
                    dfs();
                    check = j;
                }
            }
            //랜덤 하단벽 없애기
            for (int j = 0; j < M; j++) {
                random = rand() % 2;
                if (mazeSetArr[i][j] != mazeSetArr[i + 1][j]) {
                    if (random == 0) continue;
                    else {
                        mazeArr[2 * i + 2][2 * j + 1] = ' ';
                        dfs();
                    }
                }
            }
        }
    }
    //마지막 행 수직벽 없애기
    for (int j = 0; j < M-1; j++) {
        if (mazeSetArr[N - 1][j] != mazeSetArr[N - 1][j + 1]) {
            mazeArr[2 * (N - 1) + 1][2 * j + 2] = ' ';
            dfs();
        }
    }

    printMaze(fp);

    fclose(fp);
    
    return 0;
}
