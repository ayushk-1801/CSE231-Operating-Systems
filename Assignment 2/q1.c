#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void merge(int arr[], int start, int mid, int end){
    int mleft = mid - start + 1, mright = end - mid;
    int left[mleft], right[mright];

    for (int i = 0; i < mleft; i++){
        left[i] = arr[start+i];
    }
    for (int i = 0; i < mright; i++){
        right[i] = arr[mid+1+i];
    }

    int i = 0, j = 0, start1 = start;
    while(i < mleft && j < mright){
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
    while(i<mleft){
        arr[start1] = left[i];
        i++;
        start1++;
    }
    while (j<mright){
        arr[start1] = right[j];
        j++;
        start1++;
    }
}

void mergesort(int arr[], int start, int end){    
    if (start >= end){
        return;
    }
    int mid = (end-start)/2+start;
    int pipe1[2], pipe2[2];
    pipe(pipe1);
    pipe(pipe2);
    pid_t pid1 = fork();
    if (pid1 == 0) {
        close(pipe1[0]);
        mergesort(arr, start, mid);
        write(pipe1[1], arr + start, (mid-start+1) * sizeof(int));
        close(pipe1[1]);
        exit(0);
    }
    pid_t pid2 = fork();
    if (pid2 == 0) {
        close(pipe2[0]);
        mergesort(arr, mid + 1, end);
        write(pipe2[1], arr + mid + 1, (end-mid) * sizeof(int));
        close(pipe2[1]);
        exit(0);
    }
    
    close(pipe1[1]);
    close(pipe2[1]);
    wait(NULL);
    wait(NULL);

    int mleft = mid-start+1;
    int mright = end-mid;
    int lefthalf[mleft];
    int righthalf[mright];
    read(pipe1[0], lefthalf, mleft * sizeof(int));
    read(pipe2[0], righthalf, mright * sizeof(int));
    close(pipe1[0]);
    close(pipe2[0]);
    for (int i = 0; i < mleft; i++)
        arr[start+i] = lefthalf[i];
    for (int j = 0; j < mright; j++)
        arr[mid + 1 + j] = righthalf[j];
    merge(arr, start, mid, end);
}

int main(){
    int fd[2];
    if (pipe(fd) == -1){
        printf("huh");
        return 1;
    }
    int arr[16] = {16, 15, 14, 13, 12, 11 , 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    printf("Unsorted array:\n");
    for (int i = 0; i < 16; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    mergesort(arr, 0, 15);

    printf("Sorted array: [ ");
    for (int i = 0; i < 15; i++){
        printf("%d ", arr[i]);
    }
    printf("%d ]", arr[15]);
    
    return 0;
}
