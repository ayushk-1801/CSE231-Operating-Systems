#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void binarySearch(int arr[], int low, int high, int target) {
    if (low > high) {
        printf("-1\n");
        exit(0);
    }

    int mid = low + (high - low) / 2;

    if (arr[mid] == target) {
        printf("Target value's index: %d", mid);
        exit(0);
    }

    int rc = fork();

    if (rc < 0) {
        printf("Fork Failed!\n");
        exit(1);
    } else if (rc == 0) {
        if (arr[mid] > target) {
            binarySearch(arr, low, mid - 1, target);
        } else if (arr[mid] < target) {
            binarySearch(arr, mid + 1, high, target);
        }
    } else {
        wait(NULL);
        exit(0);
    }
}

int main() {
    int arr[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    int target;
    printf("Enter a target value: ");
    scanf("%d", &target);

    int low = 0, high = 15;
    binarySearch(arr, low, high, target);
    return 0;
}
