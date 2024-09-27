#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    unsigned char protectionBits;
};

struct TLBEntry {
    bool isValid;
    unsigned int vpn, pfn;
    unsigned char protectionBits;
};

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
    int n;
    printf("Enter the number of queries: ");
    scanf("%d", &n);

    char logicalAddress[10];

    for (int i = 0; i < TOTAL_PAGES; i++) {
        pageTable[i].isValid = true;
        pageTable[i].pfn = i;
        pageTable[i].protectionBits = 0;
    }

    for (int i = 0; i < TLB_SIZE; i++) {
        tlb[i].isValid = false;
    }

    while (n--) {
        printf("Enter the logical address: ");
        if (scanf("%9s", logicalAddress) != 1) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        unsigned int logical;
        char *endptr;
        logical = (unsigned int)strtol(logicalAddress, &endptr, 16);

        if (*endptr != '\0') {
            printf("Invalid input. Please enter a valid hexadecimal address.\n");
            continue;
        }

        unsigned int vpn = (logical >> SHIFT) & VPN_MASK;
        unsigned int offset = logical & OFFSET_MASK;

        struct TLBEntry tlbEntry;
        if (TLBLookup(vpn, &tlbEntry)) {
            if (tlbEntry.protectionBits == 0) {
                unsigned int physAddr = (tlbEntry.pfn << SHIFT) | offset;
                printf("TLB Hit: Physical Address = 0x%04X\n", physAddr);
            } else {
                printf("Exception: PROTECTION_FAULT\n");
            }
        } else {
            if (vpn >= TOTAL_PAGES) {
                printf("Exception: SEGMENTATION_FAULT (VPN out of range)\n");
            } else {
                struct PageTableEntry pte = pageTable[vpn];
                if (!pte.isValid) {
                    printf("Exception: SEGMENTATION_FAULT\n");
                } else if (pte.protectionBits != 0) {
                    printf("Exception: PROTECTION_FAULT\n");
                } else {
                    static int tlbIdx = 0;
                    tlb[tlbIdx].isValid = true;
                    tlb[tlbIdx].vpn = vpn;
                    tlb[tlbIdx].pfn = pte.pfn;
                    tlb[tlbIdx].protectionBits = pte.protectionBits;
                    tlbIdx = (tlbIdx + 1) % TLB_SIZE;

                    unsigned int physAddr = (pte.pfn << SHIFT) | offset;
                    printf("TLB Miss: Physical Address = 0x%04X\n", physAddr);
                }
            }
        }
    }

    return 0;
}
