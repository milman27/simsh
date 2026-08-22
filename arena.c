
#include "arena.h"

static void** arenaList = 0;

void viewMem(arena_id arena){
    struct arena_head* head  = *(arenaList + arena);
       //printf("head:%p\n size:%lu\n usage:%lu\n next:%p\n", head, head->size, head->usage, head->next);
    while(head->next){
        head = head->next;
       //printf("head:%p\n size:%lu\n usage:%lu\n next:%p\n", head, head->size, head->usage, head->next);
    }
}

void arenaFree(arena_id arena){
    if(arenaList == 0){
        //printf("Attempt to free memory before request detected.\n");
        return;
    }
    if (*((uint64_t*)arenaList + arena) == 0xCECECECECECECECE){
        //printf("Arena %d does not exist!\n", arena);
        return;
    }
    struct arena_head* next;
    struct arena_head* head = (struct arena_head*)*(arenaList + arena);
    while((next = head->next)){
        //printf("freeing %p\n", head);
        munmap(head, PAGESIZE);
        head = next;
    }
    //printf("freeing %p end\n", head);
    munmap(head, PAGESIZE);
    *((uint64_t*)arenaList + arena) = 0xCECECECECECECECE;
}

void* arenaAlloc(arena_id arena, size_t size){
    /*arena: id of arena to request memory in*/
    /*size: requested size in bytes*/
    void* returnable = 0;
    if(!arenaList) {
        if((arenaList = AAlloc(MEMUNIT))){
            memset(arenaList, 0xCE, MEMUNIT); 
        }else{
            //printf("Out of memory!\n");
        }
    }
    if (*((uint64_t*)arenaList + arena) == 0xCECECECECECECECE){
        *(arenaList + arena) = AAlloc((((size + 16)>>17) + 1)<<18);
        struct arena_head* head = *(arenaList + arena);
        head->next = 0;
        head->size = ((((size + 16)>>17) + 1)<<18) - sizeof(struct arena_head);  
        head->usage = size + 2;
        *(uint16_t*)((char*)head + (head->usage + sizeof(struct arena_head))) = 0xCECE;
        returnable = (char*)head + (sizeof(struct arena_head) + 2); 
    }else{
        struct arena_head* head = *(arenaList + arena);
        while(head->usage + size > head->size ){
           if(head->next == NULL){
                //printf("arena %d full!\n", arena);
                if((head->next = AAlloc((((size + 16)>>17) + 1)<<18))){
                    struct arena_head* next_head = head->next;
                    next_head->next = 0;
                    next_head->size = ((((size + 16)>>17) + 1)<<18) - sizeof(struct arena_head);  
                    next_head->usage = size + 2;
                    *(uint16_t*)((char*)head + (head->usage + sizeof(struct arena_head))) = 0xCECE;
                    returnable = (char*)head + (sizeof(struct arena_head) + 2); 
                    assert((head->usage < head->size));
                }else{
                    //printf("malloc failed\n");
                    return 0;
                }
            }else{
                head = head->next;
            }
        }
        if(head->usage + size < head->size){
            *(uint16_t*)((char*)head + (head->usage + sizeof(struct arena_head))) = 0xCECE;
            returnable = (char*)head + (head->usage + sizeof(struct arena_head) + 2); 
            head->usage += size + 2;
        }
    }

    return returnable;

}
