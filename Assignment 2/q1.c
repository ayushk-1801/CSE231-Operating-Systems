
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void merge(int fd[]){
    int arr[16], start, end;
    //read
    if(read(fd[0], &arr, sizeof(int)*16) < 0){
        return;
    }
    if(read(fd[0], &start, sizeof(int)) < 0){
        return;
    }
    if(read(fd[0], &end, sizeof(int)) < 0){
        return;
    }
    close(fd[0]);

    int mid = (end-start)/2+start;

    int mleft = mid - start + 1;
    int mright = end - mid;
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

    //write
    if(write(fd[1], &arr, sizeof(int)*16) < 0){
        return;
    }
    if(write(fd[1], &start, sizeof(int)) < 0){
        return;
    }
    if(write(fd[1], &end, sizeof(int)) < 0){
        return;
    }
    close(fd[1]);
}

void mergesort(int fd[]){
    int arr[16], start, end;
    //read
    if(read(fd[0], &arr, sizeof(int)*16) < 0){
        printf("huh");
        return;
    }
    if(read(fd[0], &start, sizeof(int)) < 0){
        return;
    }
    if(read(fd[0], &end, sizeof(int)) < 0){
        return;
    }
    close(fd[0]);

    int end1 = end;
    int start1 = start;
    int fork1 = fork();
    if(fork1 == -1){
        return;
    }
    else if(fork1==0){
        if(start>end){
            return;
        }
        else if(start == end){
            printf("Sorted array: %d", arr[0]);
            return;
        }
        int mid = (end-start)/2+start;
        if(write(fd[1], &arr, sizeof(int)*16) < 0){
            return;
        }
        start = start1;
        if(write(fd[1], &start, sizeof(int)) < 0){
            return;
        }
        end = mid;
        if(write(fd[1], &end, sizeof(int)) < 0){
            return;
        }
        close(fd[1]);
        printf("\naaaa\n");
        mergesort(fd);
        if(read(fd[0], &arr, sizeof(int)*16) < 0){
            return;
        }
        if(read(fd[0], &start, sizeof(int)) < 0){
            return;
        }
        if(read(fd[0], &end, sizeof(int)) < 0){
            return;
        }
        close(fd[0]);

        if(write(fd[1], &arr, sizeof(int)*16) < 0){
            return;
        }
        start = mid+1;
        if(write(fd[1], &start, sizeof(int)) < 0){
            return;
        }
        end = end1;
        if(write(fd[1], &end, sizeof(int)) < 0){
            return;
        }
        close(fd[1]);
        mergesort(fd);
        if(read(fd[0], &arr, sizeof(int)*16) < 0){
            return;
        }
        if(read(fd[0], &start, sizeof(int)) < 0){
            return;
        }
        if(read(fd[0], &end, sizeof(int)) < 0){
            return;
        }
        close(fd[0]);

        if(write(fd[1], &arr, sizeof(int)*16) < 0){
            return;
        }
        start = start1;
        if(write(fd[1], &start, sizeof(int)) < 0){
            return;
        }
        end = end1;
        if(write(fd[1], &end, sizeof(int)) < 0){
            return;
        }
        close(fd[1]);
        merge(fd);
    }
    else{
        wait(NULL);
    }
}

int main(){
    int fd[2];
    if (pipe(fd) == -1){
        printf("huh");
        return 1;
    }
    int arr[16] = {16, 15, 14, 13, 12, 11 , 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int start = 0, end = 15;
    printf("Enter 16 elements of the array: ");
    // for (int i = 0; i < 16; i++){
    //     scanf("%d", &arr[i]);
    // }

    //write
    if(write(fd[1], &arr, sizeof(int)*15) < 0){
        return 2;
    }
    if(write(fd[1], &start, sizeof(int)) < 0){
        return 3;
    }
    if(write(fd[1], &end, sizeof(int)) < 0){
        return 4;
    }
    close(fd[1]);
    printf("\nall stored\n");

    mergesort(fd);

    // //read
    if(read(fd[0], &arr, sizeof(int)*16) < 0){
        return 5;
    }
    if(read(fd[0], &start, sizeof(int)) < 0){
        return 6;
    }
    if(read(fd[0], &end, sizeof(int)) < 0){
        return 7;
    }
    close(fd[0]);

    printf("Sorted array: [ ");
    for (int i = 0; i < 15; i++){
        printf("%d, ", arr[i]);
    }
    printf("%d ]", arr[15]);
    
    return 0;
}
