#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#define PAGESIZE 4096
#define PDENTRY 1024
#define PTENTRY 1024

struct PTE{
    uint32_t PFN;
    uint32_t* values;
    int isvalid;
};
struct PDE{
    struct PTE ;
    int isvalid;
};
struct PDE Pdirectories;
// struct PTE Ptables[PDENTRY][PTENTRY];

uint8_t* physical_memory;
size_t physical_memory_size = 16 * 1024 * 1024;
// unsigned int physicalmemory[PAGESIZE*16*PTENTRY];
int Hits = 0;
int Misses = 0;

void pagefault1(uint32_t PDindex, uint32_t PTindex){
    printf("Page fault for --> PD index: %u , PT index: %u\n", PDindex, PTindex);
    Pdirectories[PDindex].PFN = PDindex;
    Pdirectories[PDindex].isvalid = true;
    Ptables[PDindex][PTindex].PFN = PTindex;
    Ptables[PDindex][PTindex].isvalid = true;
    printf("PDE PD[%u] and PTE PT[%u] set as valid\n", PDindex, PTindex);
}

void pagefault2(uint32_t PDindex, uint32_t PTindex) {
    printf("Page fault for --> PD index: %u , PT index: %u\n", PDindex, PTindex);
    if(!Ptables[PDindex][PTindex].isvalid){
        Ptables[PDindex][PTindex].PFN = PTindex;
        Ptables[PDindex][PTindex].isvalid = true;
        printf("PTE isvalidated: PT[%u] set as valid\n", PTindex);
    }
}

unsigned int load(uint32_t va) {
    uint32_t PDindex = (va & 0xFFC00000) >> 22;
    uint32_t PTindex = (va & 0x003FF000) >> 12;
    uint32_t offset = va & 0xFFF;
    
    // Check if page table exists
    if (pd->entries[pd_index] == NULL) {
        page_misses++;
        printf("Page Miss - Creating new Page Table for PD index %u\n", pd_index);
        
        // Create new page table
        pd->entries[pd_index] = (PageTable*)calloc(1, sizeof(PageTable));
        pd->entries[pd_index]->entries = (uint32_t*)calloc(PAGE_TABLE_ENTRIES, sizeof(uint32_t));
        pd->entries[pd_index]->valid = 1;
    }
    
    PageTable* pt = pd->entries[pd_index];
    
    // Check if page exists
    if (pt->entries[pt_index] == 0) {
        if (!is_write) {
            page_misses++;
            printf("Page Fault - Attempting to read from non-existent page\n");
            return -1;  // Changed to return -1 instead of exit
        }
        
        page_misses++;
        printf("Page Miss - Allocating new page for PT index %u\n", pt_index);
        
        // Allocate new page
        static uint32_t next_frame = 0;
        pt->entries[pt_index] = (next_frame++ * PAGE_SIZE) | 1;  // Set valid bit
    } else {
        page_hits++;
    }
    
    uint32_t frame = pt->entries[pt_index] & ~0xFFF;  // Clear flags
    return frame | offset;
    
    if(!Pdirectories[PDindex].isvalid){
        Misses++;
        pagefault1(PDindex, PTindex);
    }
    else if(!Ptables[PDindex][PTindex].isvalid){
        Misses++;
        pagefault2(PDindex, PTindex);
    }
    else{
        Hits++;
        uint32_t physicaladdress = (Ptables[PDindex][PTindex].PFN * PAGESIZE) + offset;
        if (physicaladdress >= PAGESIZE * PDENTRY * PTENTRY) {
            printf("Segmentation fault: Physical address out of bounds.\n");
            exit(1);
        }
        printf("Physical Address: %u", physicaladdress);
        return physicalmemory[physicaladdress];
    }
    return 0;
}

void store(uint32_t va, unsigned int value){
    uint32_t PDindex = (va >> 22) & 0x3FF;
    uint32_t PTindex = (va >> 12) & 0x3FF;
    uint32_t offset = va & 0xFFF;
    printf("moye moye: %u", offset);

    if(!Pdirectories[PDindex].isvalid){
        Misses++;
        pagefault1(PDindex, PTindex);
    }
    else if(!Ptables[PDindex][PTindex].isvalid){
        Misses++;
        pagefault2(PDindex, PTindex);
    }
    else{
        Hits++;
        uint32_t physicaladdress = (Ptables[PDindex][PTindex].PFN * PAGESIZE) + offset;
        if (physicaladdress >= PAGESIZE * PDENTRY * PTENTRY){
            printf("Segmentation fault: Physical address out of bounds.\n");
            exit(1);
        }
        physicalmemory[physicaladdress] = value;
        printf("Physical Address: %u", physicaladdress);
        return;
    }
}

void hitsandmiss(){
    printf("\nPage Hits: %d \n", Hits);
    printf("Page Misses: %d\n", Misses);
}
void hitmissratio(){
    if(Hits + Misses > 0){
        printf("Hit/Miss Ratio: %.2f\n", (float)Hits/(Hits + Misses));
    }
}

void starting(){
    int okk;
    printf("\n    1 --> Store value\n");
    printf("    2 --> Load value\n");
    printf("    3 --> Exit and show Hit/Miss\n\n");
    printf("Enter number:");
    scanf("%d", &okk);
    switch(okk){
        case 1:
            uint32_t virtualadd;
            unsigned int value;
            printf("Enter Virtual Address: ");
            scanf("%x", &virtualadd);
            printf("%x-------------------", virtualadd);
            printf("Enter value to store: ");
            scanf("%u", &value);
            printf("\n");
            store(virtualadd, value);
            hitsandmiss();
            starting();
            break;
        case 2:
            uint32_t virtualadd1;
            printf("Enter Virtual Address: ");
            scanf("%x", &virtualadd1);
            unsigned int value1 = load(virtualadd1);
            printf("\nLoaded value at %x: %u\n", virtualadd1, value1);
            hitsandmiss();
            starting();
            break;
        case 3:
            exit(0);
        default:
            printf("\nInvaild input....\n");
            // starting();
            break;
    }
}

int main() {
    for(int i = 0; i < PDENTRY; i++){
        Pdirectories[i].PFN = 0;
        Pdirectories[i].isvalid = false;
        for(int j = 0; j < PTENTRY; j++){
            Ptables[i][j].PFN = 0;
            Ptables[i][j].isvalid = false;
        }
    }
    for (int i = 0; i < (PAGESIZE * PDENTRY * PTENTRY); i++){
        physicalmemory[i] = 0;
    }
    starting();
    hitmissratio();

    //examples....
    // virtual address1 = 0x000003FF
    // virtual address2 = 0x00001000

    return 0;
}
