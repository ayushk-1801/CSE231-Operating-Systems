#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct page{
    uint8_t pageNumber;
    int referenceBit, dirtyBit;
};

void clockreplacement(struct page *frames, int numFrames, uint8_t pageNumber, int *clockPointer){
    while(true){
        if(frames[*clockPointer].referenceBit==0){
            frames[*clockPointer].pageNumber = pageNumber;
            frames[*clockPointer].referenceBit = 1;
            frames[*clockPointer].dirtyBit = 0;
            *clockPointer = (*clockPointer + 1) % numFrames;
            return;
        }
        else{
            frames[*clockPointer].referenceBit = 0;
            *clockPointer = (*clockPointer + 1) % numFrames;
        }
    }
}

int main() {
    int clockHand = 0, Faults = 0, Hits = 0, numRequests = 0, numFrames, setrequests;
    uint8_t pageRequests[1000];

    printf("Enter the number of frames: ");
    scanf("%d", &numFrames);

    struct page frames[numFrames];
    for (int i = 0; i < numFrames; i++) {
        frames[i].pageNumber = -1;
        frames[i].referenceBit = 0;
        frames[i].dirtyBit = 0;
    }

    printf("Enter number of page requests: ");
    scanf("%d", &setrequests);
    printf("Enter page requests(space separated): ");
    while(numRequests < setrequests && numRequests < 1000){
        uint8_t a;
        scanf("%hhu", &a);
        pageRequests[numRequests++] = a;
    }

    for (int i = 0; i < numRequests; i++) {
        uint8_t Requestedpage = pageRequests[i];
        int ab = 0;

        for(int j = 0; j<numFrames; j++){
            if(frames[j].pageNumber == Requestedpage){
                Hits++;
                frames[j].referenceBit = 1;
                ab = 1;
                break;
            }
        }
        if(ab==0){
            Faults++;
            clockreplacement(frames, numFrames, Requestedpage, &clockHand);
        }
    }

    printf("\n");
    printf("Total page faults: %d\n", Faults);
    printf("Total page hits: %d\n", Hits);
    printf("Hit rate: %.2f%%\n", (float)Hits/(Hits+Faults)*100);

    return 0;
}
