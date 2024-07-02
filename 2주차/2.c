// 실습 2
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

    int key, j;
    for (int i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        // 오른쪽으로 한 칸씩 땡긴다
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        // 삽입할 공간에 key값을 넣는다
        arr[j + 1] = key;
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

7
3 73 48 31 8 11 20

8
73 65 48 31 29 20 8 3
*/