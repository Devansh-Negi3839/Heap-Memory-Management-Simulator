//   *-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-     Including Libraries  *-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-

#include <stdio.h>
#include <stdlib.h>
#define HEAPSIZE 1024 // Assuming heapsize as 1024 bytes/blocks for simulation purpose

//

//   *-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-  Function Prototypes  *-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-

//

typedef enum
{
    FAILURE,
    SUCCESS
} Status_Code;

typedef struct Allocate_Node_tag // structure of allocated node
{
    int size, start, end; // start->starting address block,end->ending address of the block,size->size of the block
    struct Allocate_Node_tag *next, *prev;
    char var[10];
} Allocate_Node; // Node that is used to maintain allocated list of the heap

typedef struct Free_Node_tag // structure of free node
{
    int size, start, end; // start->starting address block,end->ending address of the block,size->size of the block
    struct Free_Node_tag *next, *prev;
} Free_Node; // Node that is used to maintain allocated list of the heap

typedef struct heap_tag // structure of heap
{
    Free_Node *free_List;         // maintaining free_List
    Allocate_Node *allocate_List; // maintaining allocated list
    int allocate_Size;            // total size allocated blocks
    int free_Size;                // total size of free free blocks
} Heap;

//

//  *-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-     Utility Functions   *-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-

//

void strcopy(char *s1, char *s2)
{
    int i = 0;
    while (s2[i] != '\0')
    {
        s1[i] = s2[i];
        i++;
    }
    s1[i] = '\0';
}

// Return 1 if not equal, 0 if equal
int strcmp(char *c1, char *c2)
{
    int retval = 0, i = 0;
    while (c1[i] != '\0' && c2[i] != '\0' && retval == 0)
    {
        if (c1[i] == c2[i])
            i++;
        else
            retval = 1;
    }
    return retval;
}

//

//  *-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-   Free_Node Function Definitions  *-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-

//

Free_Node *makeFreeNode(int start, int size) // function to make a freenode with starting of block and size of the block known
{
    Free_Node *nptr = (Free_Node *)(malloc(sizeof(Free_Node)));
    nptr->size = size;
    nptr->start = start;
    nptr->end = size + start - 1;
    nptr->next = nptr->prev = NULL;
    return nptr;
}

void addTofree_List(Free_Node **listpointer, Free_Node *fptr) // function to add the free node to the free list is in heap data structure
{
    Free_Node *ptr, *prevptr, *free_List_Head;
    free_List_Head = *listpointer;
    ptr = free_List_Head;

    if (*listpointer == NULL) // if initially no node is allocated then free_List will contain only the newly added node
        free_List_Head = fptr;
    else
    {
        while (ptr->next != NULL && ptr->start < fptr->start) // we will skip all the nodes having start address of the block less than the new free block
            ptr = ptr->next;

        if (ptr->next == NULL && ptr->start < fptr->start) // case when all nodes in free list is before newly free list i,e. adding new node as last node
        {
            ptr->next = fptr;
            fptr->prev = ptr;
        }
        else
        {
            prevptr = ptr->prev;
            // as until before point of ptr they are before newly allocated free node storing prev node
            // case when all nodes in free_List are after the newly allocated free blockor else adding to prev pointer
            if (ptr == free_List_Head)
                free_List_Head = fptr;

            if (prevptr != NULL)
                prevptr->next = fptr;

            fptr->prev = prevptr;
            fptr->next = ptr;
            ptr->prev = fptr;
        }
    }
    *listpointer = free_List_Head;
}

void deletefromfree_List(Free_Node **listpointer, Free_Node *fptr) // function to delete the freenode from the free list when free node is given
{
    Free_Node *prev_Node, *next_Node;
    prev_Node = fptr->prev;
    next_Node = fptr->next;

    if (prev_Node == NULL) // if prev node is NULL that means first node is being deleted
        *listpointer = next_Node;
    else // else conecting next node to prev node
        prev_Node->next = next_Node;

    if (next_Node != NULL) // if nextnode is not null then connecting prev to next node
        next_Node->prev = prev_Node;

    free(fptr);
    fptr = NULL;
}

//

//   *-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-    Allocate_Node Function Definitions *-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-

//

Allocate_Node *makeallocatenode(int size, char *name, int start) // function to make a allocatenode with starting of block,size of the block and variable name known
{
    Allocate_Node *nptr = (Allocate_Node *)(malloc(sizeof(Allocate_Node))); // creating a allocate node using malloc
    nptr->next = nptr->prev = NULL;
    nptr->size = size;
    nptr->start = start;
    nptr->end = start + size - 1;
    strcopy(nptr->var, name);
    return nptr; // returning allocate node after initializing
}

void addToallocate_List(Allocate_Node **listpointer, Allocate_Node *nptr) // function to add the allocate node to the allocated list that is in heap data structure
{
    Allocate_Node *allocate_Listptr, *ptr, *prevptr;
    allocate_Listptr = *listpointer;
    ptr = allocate_Listptr;
    if (allocate_Listptr == NULL) // if initially no node is allocated then allocatedlist will contain only the newly added node
        allocate_Listptr = nptr;
    else
    {
        while (ptr->next != NULL && ptr->start < nptr->start) // we will skip all the nodes having start address of the block less than the new allocated block
            ptr = ptr->next;

        if (ptr->next == NULL && ptr->start < nptr->start) // case when all nodes in allocated list is before newly allocated list i,e. adding new node as last node
        {
            ptr->next = nptr;
            nptr->prev = ptr;
        }
        else
        {
            prevptr = ptr->prev; // as until before point of ptr they are before newly allocated node storing prev node

            if (ptr == allocate_Listptr) // case when all nodes in allocated list are after the newly allocated block
                allocate_Listptr = nptr;

            if (prevptr != NULL) // or else adding to prev pointer
                prevptr->next = nptr;

            nptr->prev = prevptr;
            nptr->next = ptr;
            ptr->prev = nptr;
        }
    }
    *listpointer = allocate_Listptr; // updating the list pointer
}

void deletefromallocatedlist(Allocate_Node **listpointer, Allocate_Node *aptr) // function to delete the allocatenode from the allocatedlist when allocated node is given
{
    Allocate_Node *nextnode, *prevnode;
    prevnode = aptr->prev;
    nextnode = aptr->next;
    if (prevnode == NULL) // if prev node is NULL that means first node is being deleted
        *listpointer = nextnode;
    else // else conecting next node to prev node
        prevnode->next = nextnode;

    if (nextnode != NULL) // if nextnode is not null then connecting prev to next node
        nextnode->prev = prevnode;

    free(aptr);
    aptr = NULL;
}

// *-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-    Heap Function Definitions  *-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-

void initialize_Heap(Heap *h, int size) // Initializing heap initially as no allocated node and all are in free block
{                                       // as heap is passed by reference so no need of return type
    h->allocate_Size = 0;
    h->free_Size = HEAPSIZE;
    h->allocate_List = NULL;
    h->free_List = makeFreeNode(0, size);
}

// Option 1 declaration
Status_Code allocateUsingFirstFitAlgorithm(Heap *h, int varsize, char *name);
// Option 2 declaration
Status_Code allocateUsingBestFitAlgorithm(Heap *h, int varsize, char *name);
// Option 3 declaration
Status_Code freememoryblock(Heap *h, char *name);
// Option 4 declaration
void printheapblocks(Heap *h);
// Option 5 declaration
void printfree_List(Free_Node **freeptr);
// Option 6 declaration
void print_Allocated_List(Allocate_Node **allocateptr);
// Option 7 declaration
void freeuser_Heap(Heap *hp);

//        *-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-    Main Function   *-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-

int main()
{
    Heap user_Heap; // creating the heap with name user_Heap
    int allocated_Size;
    char var_Name[10];
    Status_Code sc;
    initialize_Heap(&user_Heap, HEAPSIZE); // initializing the heap
    int input, flag = 0;

    printf("The maximum size of the heap is %d bytes\n", HEAPSIZE);
    printf("Enter 1 to allocate using First Fit Allocation Algorithm\n");
    printf("Enter 2 to allocate using Best Fit allocation Algorithm\n");
    printf("Enter 3 to deallocate particular memory\n");
    printf("Enter 4 to display entire Heap\n");
    printf("Enter 5 to display all Free Blocks.\n");
    printf("Enter 6 to display all Allocated Blocks.\n");
    printf("Enter 0 to Terminate Program\n");
    printf("\nEnter Input :");

    scanf("%d", &input);

    while (flag == 0)
    {
        if (input == 1) // allocating based on first fit
        {
            printf("\n\n\n\n");

            printf("Enter the name of the variable: ");
            scanf("%s", var_Name);
            printf("Enter the size of the variable: ");
            scanf("%d", &allocated_Size);

            if (user_Heap.free_Size < allocated_Size) // checking whether enough space is present in heap or not
                printf("There is no enough space in the heap.\n");
            else if (allocated_Size == 0) // checking whether the allocated block have space or not
                printf("No data needs to be allocated as the size is 0 bytes\n");
            else
            {
                sc = allocateUsingFirstFitAlgorithm(&user_Heap, allocated_Size, var_Name);
                if (sc == FAILURE)
                    printf("Malloc is falied.\n");
                else
                    printf("The variable %s of size %d is allocated.\n", var_Name, allocated_Size);
            }

            printf("\n\n\n\n");
        }

        else if (input == 2) // allocating based on best fit
        {
            printf("Enter the name of the variable: ");
            scanf("%s", var_Name);
            printf("Enter the size of the variable: ");
            scanf("%d", &allocated_Size);
            if (user_Heap.free_Size < allocated_Size) // checking whether enough space is present in heap or not
            {
                printf("There is no enough space in the heap.\n");
            }
            else if (allocated_Size == 0) // checking whether the allocated block have space or not
            {
                printf("There is no data to be allocated as size is 0 bytes\n");
            }
            else
            {
                sc = allocateUsingBestFitAlgorithm(&user_Heap, allocated_Size, var_Name);
                if (sc == FAILURE)
                {
                    printf("Insufficient Memory\n");
                }
                else
                {
                    printf("The variable %s of size %d is allocated.\n", var_Name, allocated_Size);
                }
            }
            printf("\n\n\n\n");
        }

        else if (input == 3) // deallocating the space
        {
            printf("Enter the name of the variable: ");
            scanf("%s", var_Name);
            sc = freememoryblock(&user_Heap, var_Name); // calling function to free/ deallocate and storing sucess or failure
            if (sc == SUCCESS)
            {
                printf("The variable %s is deallocated is sucessfully.", var_Name);
            }
            else
            {
                printf("The variable %s is not found.", var_Name);
            }
            printf("\n\n\n\n");
        }

        else if (input == 4) // to print the heap blocks according to starting address
        {
            printf("\n\n\n\n");
            printf("The heap blocks are\n");
            printheapblocks(&user_Heap);
            printf("\n\n\n\n");
        }

        else if (input == 5) // to print free list
        {
            printf("\n\n\n\n");
            printf("The total free blocks available are %d.\n", user_Heap.free_Size);
            if (user_Heap.free_Size != 0) // if no free space is present
            {
                printf("The free blocks are\n\n");
                printfree_List(&user_Heap.free_List);
            }
            else
            {
                printf("There are no free blocks available\n");
            }

            printf("\n\n\n\n");
        }

        else if (input == 6) // to print allocated list
        {
            printf("\n\n\n\n");

            printf("The total allocated blocks are %d\n", user_Heap.allocate_Size);
            if (user_Heap.allocate_Size != 0) // if no allocated space is present
            {
                printf("The allocated blocks are\n\n");
                print_Allocated_List(&user_Heap.allocate_List);
            }
            else
                printf("There are no allocated blocks available\n");

            printf("\n\n\n\n");
        }

        else if (input == 0)
        {
            flag = 1;
            printf("Program Terminated\n");
            freeuser_Heap(&user_Heap); // deleting all the nodes that we created
        }

        else
        {
            printf("Please give correct input\n");
        }

        if (input != 0) // when user doesn't want to exit
        {
            printf("Enter 1 to allocate via first fit allocation.\nEnter 2 to allocate via first best allocation.\nEnter 3 to deallocate.\nEnter 4 to display heap.\nEnter 5 to display free blocks.\nEnter 6 to display allocated blocks.\nEnter 0 to exit the program.\n\nEnter the input :");
            scanf("%d", &input);
        }
    }
}

//*-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-

//  Option 1
Status_Code allocateUsingFirstFitAlgorithm(Heap *h, int varsize, char *name) // function to allocate a variable in heap using firstfit
{
    Free_Node *free_Listptr, *fptr;
    Allocate_Node *allocate_Listptr, *aptr;

    free_Listptr = h->free_List;
    fptr = free_Listptr;
    allocate_Listptr = h->allocate_List;
    aptr = allocate_Listptr;
    Status_Code sc = SUCCESS;
    int found = 0;

    while (fptr != NULL && found == 0) // searching till we get the enough block size that is needed to allocate
    {
        if (fptr->size >= varsize) // when we found space to allocate it
            found = 1;
        else
            fptr = fptr->next;
    }
    if (found == 0) // no enough space is found to allocate
        sc = FAILURE;
    else
    {
        Allocate_Node *newallocate = makeallocatenode(varsize, name, fptr->start); // when found creating a allocatenode to add
        addToallocate_List(&allocate_Listptr, newallocate);                        // adding to allocate list

        fptr->start = fptr->start + varsize; // updating the free node blocks
        fptr->size = fptr->size - varsize;   // updating the free node size

        h->allocate_Size = h->allocate_Size + varsize; // updating heap allocated size
        h->free_Size = h->free_Size - varsize;         // updating heap free size

        if (fptr->size == 0)                          // checking when complete free node is used or not
            deletefromfree_List(&free_Listptr, fptr); // if all blocks in free node is used we need to delete it
    }

    h->allocate_List = allocate_Listptr; // updating the  list
    h->free_List = free_Listptr;         // updating the free list

    return sc;
}

//  Option 2
Status_Code allocateUsingBestFitAlgorithm(Heap *h, int varsize, char *name) // function to allocate a variable in heap using bestfit
{
    int size = HEAPSIZE;
    Free_Node *free_Listptr, *fptr, *traverseptr;
    Allocate_Node *allocate_Listptr, *aptr;

    free_Listptr = h->free_List;
    traverseptr = free_Listptr;
    allocate_Listptr = h->allocate_List;
    aptr = allocate_Listptr;
    Status_Code sc = SUCCESS;
    int found = 0;

    while (traverseptr != NULL) // searching till we get the best enough block size and the complete list that is needed to allocate
    {
        if (traverseptr->size >= varsize) // when we found space to allocate it
        {
            found = 1;
            if (traverseptr->size < size) // checking for best fit condition
            {
                size = traverseptr->size;
                fptr = traverseptr; // storing that particular best fit node
            }
        }
        traverseptr = traverseptr->next;
    }
    if (found == 0) // no enough space is found to allocate
    {
        sc = FAILURE;
    }
    else
    {
        Allocate_Node *newallocate = makeallocatenode(varsize, name, fptr->start); // when found creating a allocatenode to add
        addToallocate_List(&allocate_Listptr, newallocate);                        // adding to allocate list

        fptr->start = fptr->start + varsize; // updating the free node blocks
        fptr->size = fptr->size - varsize;   // updating the free node size

        h->allocate_Size = h->allocate_Size + varsize; // updating heap allocated size
        h->free_Size = h->free_Size - varsize;         // updating heap free size

        if (fptr->size == 0)                          // checking when complete free node is used or not
            deletefromfree_List(&free_Listptr, fptr); // if all blocks in free node is used we need to delete it
    }

    h->allocate_List = allocate_Listptr; // updating the  list
    h->free_List = free_Listptr;         // updating the free list

    return sc;
}

//  Option 3
Status_Code freememoryblock(Heap *h, char *name) // deallocating the memory block based on variable name
{
    int found = 0;
    Status_Code sc = SUCCESS;
    Allocate_Node *aptr, *allocate_Listpointer, *prevptr;
    Free_Node *fptr, *free_Listpointer;

    allocate_Listpointer = h->allocate_List;
    aptr = allocate_Listpointer;
    free_Listpointer = h->free_List;
    fptr = free_Listpointer;

    while (aptr != NULL && found == 0) // checking for the memory block with given name using userdefined strcmp function
    {
        if (strcmp(name, aptr->var) == 0)
            found = 1;
        else
            aptr = aptr->next;
    }

    if (found == 0) // no memory block allocated with the given variable name
        sc = FAILURE;
    else // if found
    {
        h->free_Size = h->free_Size + aptr->size;         // updating heap free size
        h->allocate_Size = h->allocate_Size - aptr->size; // updating heap allocated size

        Free_Node *free_Node_Of_Delete_Alocated_Node = makeFreeNode(aptr->start, aptr->size); // creating a free node with the size of the allocated node that needed to be deleted
        deletefromallocatedlist(&allocate_Listpointer, aptr);                                 // deleting the found allocated node from allocated list
        addTofree_List(&free_Listpointer, free_Node_Of_Delete_Alocated_Node);                 // adding the created free node to free_List

        Free_Node *newfreenextnode, *newfreeprevnode;
        newfreenextnode = free_Node_Of_Delete_Alocated_Node->next;
        newfreeprevnode = free_Node_Of_Delete_Alocated_Node->prev;

        if (newfreenextnode != NULL) // checking next free node to the newly added free node is NULL or not i,e newly created node is last node or not
        {
            if (free_Node_Of_Delete_Alocated_Node->start + free_Node_Of_Delete_Alocated_Node->size == newfreenextnode->start) // if they are adjecent free nodes we need to merge it
            {
                free_Node_Of_Delete_Alocated_Node->size = free_Node_Of_Delete_Alocated_Node->size + newfreenextnode->size; // updating size
                free_Node_Of_Delete_Alocated_Node->end = newfreenextnode->end;                                             // updting the last address of the block
                deletefromfree_List(&free_Listpointer, newfreenextnode);                                                   // deleting the extra node after merging
            }
        }
        if (newfreeprevnode != NULL) // checking previous free node to the newly added free node is NULL or not i,e newly created node is first node or not
        {
            if (newfreeprevnode->start + newfreeprevnode->size == free_Node_Of_Delete_Alocated_Node->start) // if they are adjecent free nodes we need to merge it
            {
                newfreeprevnode->size = newfreeprevnode->size + free_Node_Of_Delete_Alocated_Node->size; // updating size
                newfreeprevnode->end = free_Node_Of_Delete_Alocated_Node->end;                           // updting the last address of the block
                deletefromfree_List(&free_Listpointer, free_Node_Of_Delete_Alocated_Node);               // deleting the extra node after merging
            }
        }
    }

    h->allocate_List = allocate_Listpointer; // updating the allocated list
    h->free_List = free_Listpointer;         // updating the free list

    return sc;
}

//  Option 4
void printheapblocks(Heap *h) // function to print the heap blocks details
{
    Allocate_Node *aptr = h->allocate_List;
    Free_Node *fptr = h->free_List;

    printf("Start_Address\tEnd_Address\tSize\tAllocation Status\n");
    // In the form   Start_Address   End_Address     Size    Allocation Status

    while (aptr != NULL && fptr != NULL) // until one of the lisked list is pointing to NULL  printing details in the sorted way based on starting address of blocks
    {
        if (aptr->start < fptr->start && h->allocate_Size != 0)
        {
            printf("%d\t\t%d\t\t%d\tAllocated\n", aptr->start, aptr->end, aptr->size);
            aptr = aptr->next;
        }
        else if (aptr->start > fptr->start && h->free_Size != 0)
        {
            printf("%d\t\t%d\t\t%d\tfree\n", fptr->start, fptr->end, fptr->size);
            fptr = fptr->next;
        }
    }

    if (aptr != NULL && h->allocate_Size != 0)
    {
        while (aptr != NULL)
        {
            printf("%d\t\t%d\t\t%d\tAllocated\n", aptr->start, aptr->end, aptr->size);
            aptr = aptr->next;
        }
    }
    else if (fptr != NULL && h->free_Size != 0)
    {
        while (fptr != NULL)
        {
            printf("%d\t\t%d\t\t%d\tfree\n", fptr->start, fptr->end, fptr->size);
            fptr = fptr->next;
        }
    }
}

//  Option 5
void printfree_List(Free_Node **freeptr) // function to print free list blocks
{
    Free_Node *fptr = *freeptr;
    printf("Start_Address\tEnd_Address\tSize\n"); // In the form   Start_Address   End_Address     Size
    while (fptr != NULL)
    {
        printf("%d\t\t%d\t\t%d\n", fptr->start, fptr->end, fptr->size);
        fptr = fptr->next;
    }
}

//  Option 6
void print_Allocated_List(Allocate_Node **allocateptr) // function to print allocated list
{
    Allocate_Node *aptr = *allocateptr;
    printf("Variable name\tStart_Address\tEnd_Address\tSize\n"); // In the form    Variable name   Start_Address   End_Address     Size
    while (aptr != NULL)
    {
        printf("%s\t\t%d\t\t%d\t\t%d\n", aptr->var, aptr->start, aptr->end, aptr->size);
        aptr = aptr->next;
    }
}

//  Option 7
void freeuser_Heap(Heap *hp) // freeing the linked list while exiting the program as we used heap space for creation of nodes
{
    Allocate_Node *a_List_Ptr = hp->allocate_List, *a_Ptr;

    Free_Node *f_List_Ptr = hp->free_List, *f_Ptr;

    while (a_List_Ptr != NULL) // freeing up the nodes in allocated list
    {
        a_Ptr = a_List_Ptr;
        a_List_Ptr = a_List_Ptr->next;
        free(a_Ptr);
    }

    while (f_List_Ptr != NULL) // freeing up the nodes in allocated list
    {
        f_Ptr = f_List_Ptr;
        f_List_Ptr = f_List_Ptr->next;
        free(f_Ptr);
    }

    hp->allocate_List = NULL;
    hp->free_List = NULL;
    hp->free_Size = HEAPSIZE;
    hp->allocate_Size = 0;
}

//*-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-    End of Program  *-*-*-*-*-**-*-*-*-*-*-*-*--*-*-*--*-*-*-*-*-*-*-