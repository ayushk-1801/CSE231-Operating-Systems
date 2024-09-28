#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct page {
    uint8_t pageNumber;
    bool referenceBit, dirtyBit;
};

int main() {
    int numFrames, numRequests;
    struct page *frames;
    int clockHand = 0;
    int totalFaults = 0, totalHits = 0;
    int pageRequests[1000];

    printf("Enter the number of frames: ");
    scanf("%d", &numFrames);

    frames = (struct page *)malloc(numFrames * sizeof(struct page));
    for (int i = 0; i < numFrames; i++) {
        frames[i].pageNumber = -1;
        frames[i].referenceBit = false;
        frames[i].dirtyBit = false;
    }

    printf("Enter the number of page requests: ");
    scanf("%d", &numRequests);

    printf("Enter page requests: ");
    for (int i = 0; i < numRequests; i++) {
        scanf("%d", &pageRequests[i]);
    }

    for (int i = 0; i < numRequests; i++) {
        int request = pageRequests[i];
        bool pageFound = false;

        for (int j = 0; j < numFrames; j++) {
            if (frames[j].pageNumber == request) {
                frames[j].referenceBit = true;
                totalHits++;
                pageFound = true;
                printf("Page hit: %d\n", request);
                break;
            }
        }
    }

    printf("\n");
    printf("Total page faults: %d\n", totalFaults);
    printf("Total page hits: %d\n", totalHits);
    printf("Hit rate: %.2f%%\n", (float)totalHits / (totalHits + totalFaults) * 100);

    return 0;
}
