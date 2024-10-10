// 이진탐색 - 비재귀버전
#include <stdio.h>
#include <stdlib.h>

int* arr;
int binarySearch(int n, int key);

int main() {
    int n, key;

    scanf("%d %d", &n, &key);
    arr = (int*)malloc(n * sizeof(int)); 
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf(" %d", binarySearch(n, key)); 
    free(arr);

    return 0;
}

int binarySearch(int n, int key) {
    int lowIdx = 0, highIdx = n - 1;
    int res = n; // 기본값을 n으로 설정, 찾는 값이 없을 경우 n을 반환
    while (lowIdx <= highIdx) {
        int midIdx = lowIdx + (highIdx - lowIdx) / 2;

        if (arr[midIdx] >= key) { // 중간값이 키값보다 크거나 같은 경우, 왼쪽 절반을 탐색
            highIdx = midIdx - 1;
            res = midIdx; // 현재 위치를 잠재적인 결과로 저장
        } 
        else { // 중간값이 키값보다 작은 경우, 오른쪽 절반을 탐색
            lowIdx = midIdx + 1;
        }
    }
    return res;
}

/*
8 -100
-92 -31 -7 4 14 20 29 44

8 55
-92 -31 -7 4 14 20 29 44
*/
