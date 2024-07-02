// 실습 1
#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    int temp;

    scanf("%d", &n);
    int* arr = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    int minIdx;
    for (int i = 0; i < n - 1; i++) {
        minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        temp = arr[i];
        arr[i] = arr[minIdx];
        arr[minIdx] = temp;
    }

    for (int i = 0; i < n; i++) {
        printf(" %d", arr[i]);
    }

    free(arr);
    return 0;
}

/*
5
5 4 3 2 1

8
8 31 48 73 3 65 20 29

8
73 65 48 31 29 20 8 3
*/