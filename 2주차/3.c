// 실습 3
// 처음에 배열 A와 B는 동일하게 초기화
// A배열 - 선택 정렬, B배열 - 삽입 정렬
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#pragma warning (disable:4996)

void selectionSort(int* A, int n);
void insertionSort(int* B, int n);

int main(void) {
    int n;
    LARGE_INTEGER ticksPerSec;
    LARGE_INTEGER start, end, diff;
    srand(time(NULL));

    scanf("%d", &n);
    int* A = (int*)malloc(n * sizeof(int));
    int* B = (int*)malloc(n * sizeof(int));

    int val;
    for (int i = 0; i < n; i++) {
        val = rand();
        A[i] = val;
        B[i] = val;
    }

    QueryPerformanceFrequency(&ticksPerSec);
    QueryPerformanceCounter(&start);
    selectionSort(A, n);
    QueryPerformanceCounter(&end);
    diff.QuadPart = end.QuadPart - start.QuadPart;
    printf("%.9lfms\n", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart) / 1000);

    QueryPerformanceFrequency(&ticksPerSec);
    QueryPerformanceCounter(&start);
    insertionSort(B, n);
    QueryPerformanceCounter(&end);
    diff.QuadPart = end.QuadPart - start.QuadPart;
    printf("%.9lfms\n", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart) / 1000);

    free(A);
    free(B);
    return 0;
}

void selectionSort(int* A, int n) {
    int temp;
    int minIdx;
    for (int i = 0; i < n - 1; i++) {
        minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (A[minIdx] > A[j]) {
                minIdx = j;
            }
        }
        temp = A[i];
        A[i] = A[minIdx];
        A[minIdx] = temp;
    }
}

void insertionSort(int* B, int n) {
    int key, j;
    for (int i = 1; i < n; i++) {
        key = B[i];
        j = i - 1;

        while (j >= 0 && B[j] > key) {
            B[j + 1] = B[j];
            j--;
        }

        B[j + 1] = key;
    }
}
