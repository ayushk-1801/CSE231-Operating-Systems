#include <stdio.h>
#include <stdlib.h>

#define CODE_BASE 32768
#define CODE_LIMIT 2048

#define HEAP_BASE 34816
#define HEAP_LIMIT 3072

#define STACK_BASE 28672
#define STACK_LIMIT 2048

#define SEG_MASK 0xC000
#define SEG_SHIFT 14
#define OFFSET_MASK 0x3FFF

int main() {
    unsigned int Base[] = { CODE_BASE, HEAP_BASE, STACK_BASE };
    unsigned int Bounds[] = { CODE_LIMIT, HEAP_LIMIT, STACK_LIMIT };

    char LogicalAddress[5];

    printf("Enter the logical address: ");
    scanf("%s", LogicalAddress);

    unsigned int VirtualAddress = (unsigned int)strtol(LogicalAddress, NULL, 16);

    unsigned int Segment = (VirtualAddress & SEG_MASK) >> SEG_SHIFT;
    unsigned int Offset = VirtualAddress & OFFSET_MASK;

    if (Offset >= Bounds[Segment]) {
        printf("Segmentation fault: Offset exceeds segment limit.\n");
        exit(1);
    }

    unsigned int PhysAddr = Base[Segment] + Offset;
    printf("Physical Address: %04X\n", PhysAddr);

    return 0;
}
