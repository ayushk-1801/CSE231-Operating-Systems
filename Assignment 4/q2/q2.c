#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int closestrequest(int curr, int requests[], int visited[], int size) {
    int minindex = -1, mindistance = 100000;
    for(int i = 0; i<size; i++){
        if(!visited[i]){
            int distance = abs(requests[i]-minindex);
            if (distance < mindistance) {
                mindistance = distance;
                minindex = i;
            }
        }
    }
    return minindex;
}

void SSTF(int requests[], int visited[], int size){
    // int currentsector = 100, currenttrack = 1, totalseektime = 0, nextsector, nexttrack;           change in this sector start with 0 or sometign like that
    double totalrotationallatency = 0.0;

    for (int i = 0; i < size; i++){
        int next_request = closestrequest(currentsector, requests, visited, size);
        visited[next_request] = 1;
        nextsector = requests[next_request];
        nexttrack = nextsector/100;

        int seektime = abs(nexttrack-currenttrack)*2;
        double rotationallatency = (abs(nextsector % 100 - currentsector % 100))*((60.0/72)/100)*1000;
        printf("Request %d:\n", i+1);
        printf("  Seek Time: %d ms\n", seektime);
        printf("  Rotational Latency: %.2f ms\n", rotationallatency);

        totalseektime += seektime;
        totalrotationallatency += rotationallatency;
        currentsector = nextsector;
        currenttrack = nexttrack;
    }
    printf("\nTotal Seek Time: %d ms and Total Rotational Latency: %.2f ms\n\n", totalseektime, totalrotationallatency);
}

int main(){
    int size;
    printf("Enter size of queue: ");
    scanf("%d", &size);
    int requests[size], visited[size];;
    printf("Enter %d disk requests: ", size);
    for (int i = 0; i < size; i++){
        scanf("%d", &requests[i]);
        visited[i] = 0;
    }
    // 78 289 21 495
    SSTF(requests, visited, size);
    return 0;
}