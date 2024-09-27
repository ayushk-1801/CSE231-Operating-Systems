#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 4096
#define MEMORY_SIZE 65536
#define TOTAL_PAGES (MEMORY_SIZE / PAGE_SIZE)
#define TLB_SIZE 4
#define SHIFT 12
#define VPN_MASK 0xF
#define OFFSET_MASK 0xFFF

struct PageTableEntry {
    bool isValid;
    unsigned int pfn;
    unsigned char protectBits;
};

struct TLBEntry {
    bool isValid;
    unsigned int vpn, pfn;
    unsigned char protectBits;
};

unsigned int pageTableBase;
struct PageTableEntry pageTable[TOTAL_PAGES];
struct TLBEntry tlb[TLB_SIZE];

int TLBLookup(unsigned int vpn, struct TLBEntry* entry) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].isValid && tlb[i].vpn == vpn) {
            *entry = tlb[i];
            return 1;
        }
    }
    return 0;
}

int main() {
    char logicalAddress[5];

    pageTableBase = 0;
    for (int i = 0; i < TOTAL_PAGES; i++) {
        pageTable[i].isValid = true;
        pageTable[i].pfn = i;
        pageTable[i].protectBits = 0;
    }
    for (int i = 0; i < TLB_SIZE; i++) {
        tlb[i].isValid = false;
    }

    printf("Enter the logical address: ");
    scanf("%4s", logicalAddress);

    unsigned int logical = (unsigned int)strtol(logicalAddress, NULL, 16);
    unsigned int vpn = (logical & VPN_MASK) >> SHIFT;
    unsigned int offset = logical & OFFSET_MASK;

    struct TLBEntry tlbEntry;

    if (TLBLookup(vpn, &tlbEntry)) {
        if (tlbEntry.protectBits == 0) {
            unsigned int physAddr = (tlbEntry.pfn << SHIFT) | offset;
            printf("TLB Hit: Physical Address = %04X\n", physAddr);
        } else {
            printf("Exception: PROTECTION_FAULT\n");
        }
    } else {
        unsigned int pteAddr = pageTableBase + (vpn * sizeof(struct PageTableEntry));
        struct PageTableEntry pte = pageTable[vpn];

        if (!pte.isValid) {
            printf("Exception: SEGMENTATION_FAULT\n");
        } else if (pte.protectBits != 0) {
            printf("Exception: PROTECTION_FAULT\n");
        } else {
            static int tlbIdx = 0;
            tlb[tlbIdx].isValid = true;
            tlb[tlbIdx].vpn = vpn;
            tlb[tlbIdx].pfn = pte.pfn;
            tlb[tlbIdx].protectBits = pte.protectBits;
            tlbIdx = (tlbIdx + 1) % TLB_SIZE;
            unsigned int physAddr = (pte.pfn << SHIFT) | offset;
            printf("TLB Miss: Physical Address = %04X\n", physAddr);
        }
    }

    return 0;
}
