// 이진탐색 - 재귀버전
#include <stdio.h>
#include <stdlib.h>

int* arr;
int binarySearch(int lowIdx, int highIdx, int key);

int main() {
    int n, key;

    scanf("%d %d", &n, &key);
    arr = (int*)malloc(n * sizeof(int)); 
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    printf(" %d", binarySearch(0, n - 1, key)); 
    free(arr);

    return 0;
}

int binarySearch(int lowIdx, int highIdx, int key) {
    if (lowIdx > highIdx) {
        if (highIdx < 0) { // 찾고자 하는 값보다 작은 값이 없는 경우
            return -1; 
        }
        return highIdx; // 가장 가까운 작은 값의 인덱스를 반환
    }

    int midIdx = lowIdx + (highIdx - lowIdx) / 2;
    if (arr[midIdx] > key) { // 중간값이 키값보다 큰 경우, 왼쪽 절반을 탐색
        return binarySearch(lowIdx, midIdx - 1, key);
    }
    else if (arr[midIdx] < key) { // 중간값이 키값보다 작은 경우, 오른쪽 절반을 탐색
        return binarySearch(midIdx + 1, highIdx, key);
    } 
    else if (arr[midIdx] == key) {
        return midIdx;
    } 
}

/*
8 -7
-92 -31 -7 4 14 20 29 44

8 33
-92 -31 -7 4 14 20 29 44
*/
