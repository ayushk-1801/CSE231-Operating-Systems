#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGESIZE 4096
#define PDENTRY 1024
#define PTENTRY 1024

struct PTE {
    uint32_t PFN;    // Physical Frame Number
    int isvalid;
};

struct PDE {
    struct PTE *values[PTENTRY];
    int isvalid;
};

// Global variables as in original code
struct PDE *Pdirectories;
uint8_t *physical_memory;
size_t physical_memory_size = 16 * 1024 * 1024;
int Hits = 0;
int Misses = 0;

uint32_t allocate_physical_frame() {
    static uint32_t next_frame = 0;
    uint32_t frame = next_frame * PAGESIZE;
    next_frame = (next_frame + 1) % (physical_memory_size / PAGESIZE);
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

    struct PTE *pt_entry = Pdirectories[PDindex].values[PTindex];
    if (!pt_entry->isvalid) {
        Misses++;
        printf(" -> Page Fault at PT index: %u\n", PTindex);
        return 0;
    }

    Hits++;
    printf(" -- Page Hit at PD index: %u, PT index: %u\n", PDindex, PTindex);
    
    uint32_t physical_addr = pt_entry->PFN + offset;
    return physical_memory[physical_addr];
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
            fprintf(stderr, "Failed to allocate PTE\n");
            exit(1);
        }
        Pdirectories[PDindex].values[PTindex]->PFN = allocate_physical_frame();
        Pdirectories[PDindex].values[PTindex]->isvalid = 1;
    } else {
        Hits++;
        printf(" -- Page Hit at PD index: %u, PT index: %u\n", PDindex, PTindex);
    }

    uint32_t physical_addr = Pdirectories[PDindex].values[PTindex]->PFN + offset;
    physical_memory[physical_addr] = value;
}

void cleanup() {
    for (int i = 0; i < PDENTRY; i++) {
        for (int j = 0; j < PTENTRY; j++) {
            free(Pdirectories[i].values[j]);
        }
    }
    free(Pdirectories);
    free(physical_memory);
}

void hitsandmiss() {
    printf("\n- Page Hits: %d\n", Hits);
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
    while (1) {
        printf("\n    1 --> Store value\n");
        printf("    2 --> Load value\n");
        printf("    3 --> Exit and show Hit/Miss\n\n");
        
        int choice;
        printf("Enter number: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');  // Clear input buffer
            printf("\nInvalid input. Please enter a number.\n");
            continue;
        }

        if (choice == 3) return;

        uint32_t address;
        printf("Enter virtual address (e.g. CCC0FFEE): 0x");
        if (scanf("%x", &address) != 1) {
            while (getchar() != '\n');
            printf("\nInvalid address format.\n");
            continue;
        }

        if (choice == 1) {
            unsigned int value;
            printf("Enter value to store (0-255): ");
            if (scanf("%u", &value) != 1 || value > 255) {
                while (getchar() != '\n');
                printf("\nInvalid value. Must be 0-255.\n");
                continue;
            }
            store(address, (uint8_t)value);
            printf(" -- Value %u stored at address 0x%08X\n", value, address);
        } else if (choice == 2) {
            uint8_t value = load(address);
            printf(" -- Value at address 0x%08X: %u\n", address, value);
        }
        
        hitsandmiss();
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
    cleanup();

    return 0;
}