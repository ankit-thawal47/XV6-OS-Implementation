//
//

#include "../h/MemoryAllocator.hpp"
#include "../tests/printing.hpp"

MemoryPiece *MemoryAllocator::free = nullptr;

MemoryPiece *MemoryAllocator::allocated = nullptr;

uint64 MemoryAllocator::allocd = 0;

uint64 MemoryAllocator::deallocd = 0;

void MemoryAllocator::initialise_memory()
{
    free = (MemoryPiece*)HEAP_START_ADDR;

    MemoryPiece  *mem = free;

    mem->prev=mem->next= nullptr;
    mem->size = (size_t)((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - MEM_BLOCK_SIZE);
    mem->status=FREE;
}

void *MemoryAllocator::alloc(size_t requested_size)
{
//if the number of blocks is not divisible by the size of the block, do peding
    if(requested_size % MEM_BLOCK_SIZE != 0)
        requested_size = (requested_size / MEM_BLOCK_SIZE + 1 ) * MEM_BLOCK_SIZE;


//if the allocation is successful, the address of the starting block is returned

    for (MemoryPiece* current = free; current; current=current->next)
    {
        if (current->size==requested_size)
        {
            remove(&free, current);
            insert(&allocated, current, ALLOCATED);
            allocd+=requested_size;
            return (void*)((char*)current + sizeof(MemoryPiece));
        }

            /*if the free segment is larger than the required amount of memory, then this segment
             * breaks it into two parts, one new segment will be allocated and the required size while the excess will be
             * that remains in the second segment will be returned to the list of free segments*/

        else if (current->size > requested_size)
        {

            remove(&free, current);

            //This new address is the one obtained when the original segment was cut at the calculated place

            MemoryPiece* new_node = (MemoryPiece*)((char*)current + sizeof(MemoryPiece) + requested_size);

            new_node->prev=new_node->next= nullptr;
            new_node->size = (current->size-sizeof(MemoryPiece)-requested_size);

            current->size = requested_size;

            insert(&free, new_node, FREE);
            insert(&allocated, current, ALLOCATED);

            char* returnval = (char*)current+sizeof(MemoryPiece);
            allocd+=requested_size;

            return (void*)(returnval);

        }
    }
    return nullptr; //there is no corresponding segment in memory
}

int MemoryAllocator::mem_free(void *mem_to_free)
{
    if(!mem_to_free)
        return -1;

    MemoryPiece * mem_desc = (MemoryPiece*)((char*)mem_to_free-sizeof(MemoryPiece));

    if(mem_desc->status != ALLOCATED)
        return -1;

    deallocd+=mem_desc->size;

    remove(&allocated, mem_desc);
    insert(&free, mem_desc, FREE);
    //calling the concatenation method after each memory release to avoid fragmentation
    merge_blocks(mem_desc);

    return 1;

}
