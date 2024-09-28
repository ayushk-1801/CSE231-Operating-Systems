#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#define PAGESIZE 4096
#define PDENTRY 1024
#define PTENTRY 1024

struct PTE{
    unsigned int PFN;
    bool isvalid;
};
struct PDE{
    unsigned int PFN;
    bool isvalid;
};
struct PDE Pdirectories[PDENTRY];
struct PTE Ptables[PDENTRY][PTENTRY];
uint8_t physicalmemory[PAGESIZE*PDENTRY*PTENTRY];
int Hits = 0;
int Misses = 0;

void pagefault(uint32_t PDindex, uint32_t PTindex) {
    printf("Page fault for --> PD index: %u , PT index: %u\n", PDindex, PTindex);

    if (!Pdirectories[PDindex].isvalid){
        Pdirectories[PDindex].PFN = PDindex;
        Pdirectories[PDindex].isvalid = true;
        Ptables[PDindex][PTindex].PFN = PTindex;
        Ptables[PDindex][PTindex].isvalid = true;
        printf("PDE PD[%u] and PTE PT[%u] set as valid\n", PDindex, PTindex);
        return;
    }
    if(!Ptables[PDindex][PTindex].isvalid){
        Ptables[PDindex][PTindex].PFN = PTindex;
        Ptables[PDindex][PTindex].isvalid = true;
        printf("PTE isvalidated: PT[%u] set as valid\n", PTindex);
        return;
    }
}

uint8_t load(uint32_t va) {
    uint32_t PDindex = (va >> 22) & 0x3FF;
    uint32_t PTindex = (va >> 12) & 0x3FF;
    uint32_t offset = va & 0xFFF;

    if((!Pdirectories[PDindex].isvalid) || (!Ptables[PDindex][PTindex].isvalid)){
        Misses++;
        pagefault(PDindex, PTindex);
    }
    else{
        Hits++;
        uint32_t physicaladdress = (Ptables[PDindex][PTindex].PFN * PAGESIZE) + offset;
        printf("Physical Address: %u", physicaladdress);
        return physicalmemory[physicaladdress];
    }
    return -1;
}

void store(uint32_t va, unsigned int value){
    uint32_t PDindex = (va >> 22) & 0x3FF;
    uint32_t PTindex = (va >> 12) & 0x3FF;
    uint32_t offset = va & 0xFFF;

    if ((!Pdirectories[PDindex].isvalid) || (!Ptables[PDindex][PTindex].isvalid)){
        Misses++;
        pagefault(PDindex, PTindex);
        return;
    }
    else{
        Hits++;
        uint32_t physicaladdress = (Ptables[PDindex][PTindex].PFN * PAGESIZE) + offset;
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
            printf("Enter value to store: ");
            scanf("%d\n", &value);
            store(virtualadd, value);
            hitsandmiss();
            starting();
            break;
        case 2:
            uint32_t virtualadd1;
            printf("Enter Virtual Address: ");
            scanf("%x", &virtualadd1);
            unsigned int value1 = load(virtualadd1);
            printf("\nLoaded value at %u: %u\n", virtualadd1, value1);
            hitsandmiss();
            starting();
            break;
        case 3:
            exit(0);
        default:
            printf("\nInvaild input....\n");
            starting();
            break;
    }
}

int main() {
    for(int i = 0; i < PTENTRY; i++){
        Pdirectories[i].PFN = 0;
        Pdirectories[i].isvalid = false;
        for(int j = 0; j < PDENTRY; j++){
            Ptables[i][j].PFN = 0;
            Ptables[i][j].isvalid = false;
        }
    }
    for (int i = 0; i < sizeof(physicalmemory); i++){
        physicalmemory[i] = 0;
    }
    starting();
    hitmissratio();

    //examples....
    // virtual address1 = 0x000003FF
    // virtual address2 = 0x00001000

    return 0;
}
