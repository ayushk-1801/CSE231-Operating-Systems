#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGESIZE 4096
#define PDENTRY 1024
#define PTENTRY 1024

struct PTE {
    uint32_t PFN;
    uint32_t *values;
    int isvalid;
};
struct PDE {
    struct PTE *values[PTENTRY];
    int isvalid;
};
struct PDE *Pdirectories;

uint8_t *physical_memory;
size_t physical_memory_size = 16 * 1024 * 1024;
int Hits = 0;
int Misses = 0;

unsigned int load(uint32_t va) {
    uint32_t PDindex = (va & 0xFFC00000) >> 22;
    uint32_t PTindex = (va & 0x003FF000) >> 12;
    uint32_t offset = va & 0xFFF;

    if (Pdirectories->values[PDindex] == NULL) {
        Misses++;
        printf(" -> Page Fault at PD index: %u\n", PDindex);

        Pdirectories->values[PDindex] = (struct PTE *)calloc(1, sizeof(struct PTE));
        Pdirectories->values[PDindex]->values =
            (uint32_t *)calloc(PTENTRY, sizeof(uint32_t));
        Pdirectories->values[PDindex]->isvalid = 1;
    }
    struct PTE *pt = Pdirectories->values[PDindex];
    if (pt->values[PTindex] == 0) {
        Misses++;
        printf(" -> Page Fault at PT index: %u\n", PTindex);
        return 0;
    } else {
        Hits++;
        printf(" -- Page Hit at PD index: %u, PT index: %u\n", PDindex, PTindex);
    }

    uint32_t frame = pt->values[PTindex] & ~0xFFF;
    uint32_t index = frame;
    return physical_memory[index];
}

void store(uint32_t va, unsigned int value) {
    uint32_t PDindex = (va & 0xFFC00000) >> 22;
    uint32_t PTindex = (va & 0x003FF000) >> 12;
    uint32_t offset = va & 0xFFF;

    if (Pdirectories->values[PDindex] == NULL) {
        Misses++;
        printf(" -> Page Miss at PD index: %u\n", PDindex);

        Pdirectories->values[PDindex] = (struct PTE *)calloc(1, sizeof(struct PTE));
        Pdirectories->values[PDindex]->values =
            (uint32_t *)calloc(PTENTRY, sizeof(uint32_t));
        Pdirectories->values[PDindex]->isvalid = 1;
    }
    struct PTE *pt = Pdirectories->values[PDindex];
    if (pt->values[PTindex] == 0) {
        Misses++;
        printf(" -> Page Miss at PT index: %u\n", PTindex);
        pt->values[PTindex] = 1;
    } else {
        Hits++;
        printf(" -- Page Hit at PD index: %u, PT index: %u\n", PDindex, PTindex);
    }
    uint32_t frame = pt->values[PTindex] & ~0xFFF;
    uint8_t ind = frame;
    physical_memory[ind] = value;
}

void hitsandmiss() {
    printf("\n- Page Hits: %d \n", Hits);
    printf("- Page Misses: %d\n", Misses);
}
void hitmissratio() {
    hitsandmiss();
    if (Hits + Misses > 0) {
        printf("- Hit/Miss Ratio: %.2f\n", (float)Hits / (Hits + Misses));
    }
    printf("- Page Directory Size: %lu bytes\n", sizeof(struct PDE));
    printf("- Page Table Size: %lu bytes\n", sizeof(struct PTE));
}

void starting() {
    int okk;
    uint32_t address;
    uint8_t value;

    printf("\n    1 --> Store value\n");
    printf("    2 --> Load value\n");
    printf("    3 --> Exit and show Hit/Miss\n\n");
    printf("Enter number: ");
    scanf("%d", &okk);
    switch (okk) {
        case 1:
            printf("Enter virtual address(e.g. CCC0FFEE): 0x");
            scanf("%x", &address);
            printf("Enter value to store(0-255): ");
            unsigned int value;
            scanf("%u", &value);
            store(address, (uint8_t)value);
            printf(" -- Value %u stored at address 0x%08X....\n", value, address);
            hitsandmiss();
            starting();
            break;
        case 2:
            printf("Enter virtual address(e.g. CCC0FFEE): 0x");
            scanf("%x", &address);
            value = load(address);
            printf(" -- Value at address 0x%08X: %u\n", address, value);
            hitsandmiss();
            starting();
            break;
        case 3:
            return;
        default:
            printf("\nInvaild input....\n");
            starting();
            break;
    }
}

int main() {
    Pdirectories = (struct PDE *)calloc(1, sizeof(struct PDE));
    physical_memory = (uint8_t *)calloc(physical_memory_size, sizeof(uint8_t));

    starting();
    hitmissratio();

    return 0;
}
