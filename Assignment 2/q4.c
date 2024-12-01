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

uint32_t physicalframe(){
    static uint32_t nextFrame = 0;
    uint32_t frame = nextFrame*PAGESIZE;
    nextFrame = (nextFrame+1) % (physical_memory_size/PAGESIZE);
    return frame;
}

unsigned int load(uint32_t va) {
    uint32_t PDindex = (va >> 22) & 0x3FF;
    uint32_t PTindex = (va >> 12) & 0x3FF;
    uint32_t offset = va & 0xFFF;

    if (Pdirectories[PDindex].values[PTindex] == NULL) {
        Misses++;
        printf(" -> Page Fault at PD index: %u\n", PDindex);
        return 0;
    }
    struct PTE *pt = Pdirectories[PDindex].values[PTindex];
    if (pt->isvalid == 0){
        Misses++;
        printf(" -> Page Fault at PT index: %u\n", PTindex);
        return 0;
    }
    Hits++;
    printf(" -- Page Hit at PD index: %u, PT index: %u\n", PDindex, PTindex);

    uint32_t physicaladdress = pt->PFN+offset;
    return physical_memory[physicaladdress];
}

void store(uint32_t va, unsigned int value) {
    uint32_t PDindex = (va >> 22) & 0x3FF;
    uint32_t PTindex = (va >> 12) & 0x3FF;
    uint32_t offset = va & 0xFFF;

    if (Pdirectories[PDindex].values[PTindex] == NULL) {
        Misses++;
        printf(" -> Page Miss at PD index: %u\n", PDindex);
        Pdirectories[PDindex].values[PTindex] = malloc(sizeof(struct PTE));
        if (!Pdirectories[PDindex].values[PTindex]) {
            printf(" -- Failed to allocate PTE\n");
            exit(1);
        }
        Pdirectories[PDindex].values[PTindex]->PFN = physicalframe();
        Pdirectories[PDindex].values[PTindex]->isvalid = 1;
    }
    else{
        Hits++;
        printf(" -- Page Hit at PD index: %u, PT index: %u\n", PDindex, PTindex);
    }
    uint32_t physicaladdress = Pdirectories[PDindex].values[PTindex]->PFN + offset;
    physical_memory[physicaladdress] = value;
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

void clean() {
    for (int i = 0; i < PDENTRY; i++) {
        for (int j = 0; j < PTENTRY; j++) {
            free(Pdirectories[i].values[j]);
        }
    }
    free(Pdirectories);
    free(physical_memory);
}

void starting(){
    int okk;
    uint32_t address;

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
            uint8_t value2 = load(address);
            printf(" -- Value at address 0x%08X: %u\n", address, value2);
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
    Pdirectories = calloc(PDENTRY, sizeof(struct PDE));
    physical_memory = calloc(physical_memory_size, sizeof(uint8_t));

    if (!Pdirectories || !physical_memory) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 1;
    }

    starting();
    hitmissratio();
    clean();

    return 0;
}
