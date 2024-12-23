#include <stdio.h>
#include <stdlib.h>

#define CODE_BASE 32768
#define CODE_BOUND 2048

#define HEAP_BASE 34816
#define HEAP_BOUND 3072

#define STACK_BASE 28672
#define STACK_BOUND 2048

#define SEG_MASK 0xC000
#define SEG_SHIFT 14
#define OFFSET_MASK 0x3FFF

int main() {
    unsigned int base[] = { CODE_BASE, HEAP_BASE, STACK_BASE };
    unsigned int bounds[] = { CODE_BOUND, HEAP_BOUND, STACK_BOUND };

    char logicalAddress[5];

    printf("Enter the logical address: ");
    scanf("%s", logicalAddress);

    unsigned int virtualAddress = (unsigned int)strtol(logicalAddress, NULL, 16);

    unsigned int segment = (virtualAddress & SEG_MASK) >> SEG_SHIFT;
    unsigned int offset = virtualAddress & OFFSET_MASK;

    if (offset >= bounds[segment]) {
        printf("Segmentation fault!\n");
        exit(1);
    }

    unsigned int physAddr = base[segment] + offset;
    printf("Physical Address: %04X\n", physAddr);

    return 0;
}
