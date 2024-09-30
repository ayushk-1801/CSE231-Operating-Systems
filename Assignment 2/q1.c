#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void merge(int arr[], int s, int mid, int e){
    int l = mid - s + 1, r = e - mid;
    int left[l], right[r];

    for (int i = 0; i < l; i++){
        left[i] = arr[s+i];
    }
    for (int i = 0; i < r; i++){
        right[i] = arr[mid+1+i];
    }

    int i = 0, j = 0, start1 = s;
    while(i < l && j < r){
        if(left[i]<=right[j]){
            arr[start1] = left[i];
            i++;
        }
        else{
            arr[start1] = right[j];
            j++;
        }
        start1++;
    }
    while(i<l){
        arr[start1] = left[i];
        i++;
        start1++;
    }
    while (j<r){
        arr[start1] = right[j];
        j++;
        start1++;
    }
}

void mergeSort(int arr[], int s, int e){
    if (s >= e){
        return;
    }
    int mid = (e-s)/2+s;
    int pipe1[2], pipe2[2];
    pipe(pipe1);
    pipe(pipe2);
    if (pipe1 == -1 || pipe2 == -1){
        printf("Pipe failed\n");
        return;
    }
    pid_t pid1 = fork();
    if (pid1 == 0) {
        close(pipe1[0]);
        mergeSort(arr, s, mid);
        write(pipe1[1], arr + s, (mid-s+1) * sizeof(int));
        close(pipe1[1]);
        exit(0);
    }
    pid_t pid2 = fork();
    if (pid2 == 0) {
        close(pipe2[0]);
        mergeSort(arr, mid + 1, e);
        write(pipe2[1], arr + mid + 1, (e-mid) * sizeof(int));
        close(pipe2[1]);
        exit(0);
    }

    close(pipe1[1]);
    close(pipe2[1]);
    wait(NULL);
    wait(NULL);

    int l = mid-s+1, r = e-mid;
    int left[l];
    int right[r];
    read(pipe1[0], left, l * sizeof(int));
    read(pipe2[0], right, r * sizeof(int));
    close(pipe1[0]);
    close(pipe2[0]);
    for (int i = 0; i < l; i++){
        arr[s+i] = left[i];
    }
    for (int j = 0; j < r; j++){
        arr[mid + 1 + j] = right[j];
    }
    merge(arr, s, mid, e);
}

int main(){
    int arr[16] = {16, 5, 14, 130, 11, 1000 , 90, 9, 4, 7, 26, 19, 48, 3, 1, 20};
    
    printf("Unsorted array:\n");
    for (int i = 0; i < 16; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    mergeSort(arr, 0, 15);

    printf("Sorted array: [ ");
    for (int i = 0; i < 15; i++){
        printf("%d ", arr[i]);
    }
    printf("%d ]", arr[15]);

    return 0;
}
