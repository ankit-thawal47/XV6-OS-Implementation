//
//

#include "../h/MemoryPiece.hpp"

void insert(MemoryPiece **head_pointer, MemoryPiece *node, MemoryPieceStatus status)
{
    if (!node) return;

    node->status = status;

    if (!(*head_pointer)) {
        *head_pointer = node;
        return;
    }

   
    MemoryPiece *head = *head_pointer;
    if(node < head)
    {
        node->next = head;
        head->prev = node;
        *head_pointer = node;

        return;
    }


    MemoryPiece  *prev = head;
    for (MemoryPiece *current = head->next; current; current = current->next)
    {
        if (node < current)
        {
            prev->next = node;
            node->prev = prev;

            current->prev = node;
            node ->next = current;
            return;
        }
        prev=current;
    }

    prev->next = node;
    node->prev = prev;
}

void remove(MemoryPiece **head_pointer, MemoryPiece* node)
{
    if(!head_pointer || !node) return; 

    MemoryPiece *head = *head_pointer;
    if (head==node)
    {
        *head_pointer = head->next;
        if(*head_pointer) 
        {
            (*head_pointer)->prev = nullptr;
        }
        node->next = node->prev=nullptr;
        return;
    }



    MemoryPiece *prev = node ->prev;
    MemoryPiece *next = node ->next;

    prev->next=next;
    if(next)
        next->prev=prev;

    node->prev=node->next=nullptr;
}

void merge_blocks(MemoryPiece *free)
{
    if(!free||free->status!=FREE) return;

    MemoryPiece *next, *current;

    if(free->prev)
        current=free->prev;
    else
        current = free;


    while (current->next && (((char*)current + current->size + sizeof(MemoryPiece)) >= ((char*)(current->next))))
    {
        next=current->next;

        current->size += sizeof(MemoryPiece) + next->size;
        current->next = current->next->next;

        if(current->next)
            current->next->prev=current;
    }

}