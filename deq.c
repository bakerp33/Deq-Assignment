#include "deq.h"
#include <stdio.h>
#include <stdlib.h>


/*This function initializes a new VoidDeq and records its pointer in
(*vDeqPtrPtr)*/

int vDeqInit(VoidDeq** vDeqPtrPtr)
{
    
    //vDeqPtr = name of pointer to deq, of type VoidDeq*
    //vDeqPtrPtr = name of pointer to pointer of deq with type VoidDeq**
    if (vDeqPtrPtr == NULL)
    {   return NULL_PARAM; } 
    if (*vDeqPtrPtr != NULL)
    {   return BAD_INIT_PARAM; }
    
    
    (*vDeqPtrPtr) = (VoidDeq*) myMalloc(sizeof(VoidDeq));
    if ((*vDeqPtrPtr) == NULL) // test malloc return
    { return OUT_OF_MEMORY; }
    
    (*vDeqPtrPtr)->headPtr = NULL;
    (*vDeqPtrPtr)->tailPtr = NULL;
    return SUCCESS;
}

/* creates a new VoidDeqNode, stores datPtr in it and adds the node as a new head*/

int vDeqPushHead(VoidDeq* vDeqPtr, void* dataPtr)
{
    if (vDeqPtr == NULL)
    {   return NULL_PARAM; }
     
  VoidDeqNode* newHead = (VoidDeqNode*) myMalloc(sizeof(VoidDeqNode));
  if ((newHead) == NULL)
  { 
      return OUT_OF_MEMORY;
  }
  
  newHead->dataPtr = dataPtr;
  newHead->nextPtr = vDeqPtr->headPtr;
  newHead->prevPtr = NULL;
  
  if (vDeqPtr ->headPtr == NULL)    // check for  empty queue
  {
      vDeqPtr->headPtr = newHead;
      vDeqPtr->tailPtr = newHead;
  }
  else
  {
      vDeqPtr->tailPtr->prevPtr = newHead;
      vDeqPtr->headPtr = newHead;
  }
  
  return SUCCESS;
}

/* creates new node and pushes it on the tail*/

int vDeqPushTail(VoidDeq* vDeqPtr, void* dataPtr)
{
    
    if (vDeqPtr == NULL)
    {   return NULL_PARAM; }
    
  VoidDeqNode* newTail = (VoidDeqNode*) myMalloc(sizeof(VoidDeqNode));
  if ((newTail) == NULL)
  {
      return OUT_OF_MEMORY;
  }
  
  newTail->dataPtr = dataPtr;
  newTail->prevPtr = vDeqPtr->tailPtr;
  newTail->nextPtr = NULL;
  
  // check for empty queue
  if (vDeqPtr->headPtr == NULL)
  {
      vDeqPtr->headPtr = newTail;
      vDeqPtr->tailPtr = newTail;
  }
  else
  {
      vDeqPtr->tailPtr->nextPtr = newTail;
      vDeqPtr->tailPtr = newTail;
  }

  return SUCCESS;

}



int vDeqPopHead(VoidDeq* vDeqPtr, void** dataPtrPtr)
{
    if (vDeqPtr == NULL || dataPtrPtr == NULL)
    {
        return NULL_PARAM;
    }
    if (*dataPtrPtr != NULL)
    {
        return BAD_POP_DATA_PARAM;
    }
    
    else if (vDeqPtr->headPtr == NULL)
    {
        return POP_EMPTY_DEQ;
    }
  (*dataPtrPtr) = vDeqPtr->headPtr->dataPtr;   // pointer to data of head
  VoidDeqNode* oldHead = vDeqPtr->headPtr;    //store pointer to the current head
  
  
  vDeqPtr->headPtr = vDeqPtr->headPtr->nextPtr;   //set the head of queue to the next node
  
  if (vDeqPtr->headPtr != NULL)
  {
    vDeqPtr->headPtr->prevPtr = NULL;
  }
   
  if (vDeqPtr->headPtr == NULL)
  {
      vDeqPtr->tailPtr = NULL;
  }
  
  oldHead->nextPtr = NULL;
  oldHead->dataPtr = NULL;
  free(oldHead);
  return SUCCESS;

}

int vDeqPopTail(VoidDeq* vDeqPtr, void** dataPtrPtr)
{
    
    if (vDeqPtr == NULL || dataPtrPtr == NULL)
    {
        return NULL_PARAM;
    }
    
    if (*dataPtrPtr != NULL)
    {
        return BAD_POP_DATA_PARAM;
    }
    
    else if (vDeqPtr->headPtr == NULL)
    {
        return POP_EMPTY_DEQ;
    }

  (*dataPtrPtr) = vDeqPtr->tailPtr->dataPtr;   // pointer to data of the tail
  VoidDeqNode* oldTail = vDeqPtr->tailPtr;    //store pointer to the current tail
  vDeqPtr->tailPtr = vDeqPtr->tailPtr->prevPtr;   //set the tail of queue to the previous node

  if (vDeqPtr->tailPtr != NULL)
  {
    vDeqPtr->tailPtr->nextPtr = NULL;
  }
   
  if (vDeqPtr->tailPtr == NULL)
  {
      vDeqPtr->headPtr = NULL;
  }
  
  oldTail->prevPtr = NULL;
  oldTail->dataPtr = NULL;
  free(oldTail);
  return SUCCESS;

}

/*This function records the dataPtr of the current head of vDeqPtrinto (*dataPtrPtr) */

int vDeqPeekHead(VoidDeq* vDeqPtr, void** dataPtrPtr)
{
    if (vDeqPtr == NULL || dataPtrPtr == NULL)
    {
        return NULL_PARAM;
    }
    
    if (*dataPtrPtr != NULL)
    {
        return BAD_PEEK_DATA_PARAM;
    }
    
    else if (vDeqPtr->headPtr == NULL)
    {
        return PEEK_EMPTY_DEQ;
    }

  (*dataPtrPtr) = vDeqPtr->headPtr->dataPtr;  // point to data of head
  return SUCCESS;
}

/*This function records the dataPtrof the current tail of vDeqPtr into (*dataPtrPtr) */
int vDeqPeekTail(VoidDeq* vDeqPtr, void** dataPtrPtr)
{
    if (vDeqPtr == NULL || dataPtrPtr == NULL)
    {
        return NULL_PARAM;
    }
    
    if (*dataPtrPtr != NULL)
    {
        return BAD_PEEK_DATA_PARAM;
    }
    
    else if (vDeqPtr->headPtr == NULL)
    {
        return PEEK_EMPTY_DEQ;
    }

  (*dataPtrPtr) = vDeqPtr->tailPtr->dataPtr;  // point to data of the tail
  return SUCCESS;
}

/* This function frees all memory used by vDeqPtr */
int vDeqFree(VoidDeq* vDeqPtr, int freeDatas)
{
    if (vDeqPtr == NULL)
    {
        return NULL_PARAM;
    }
    
    if (freeDatas == 0)
    {
        free(vDeqPtr);
    }
    else if (freeDatas == 1)
    {
        while (vDeqPtr->headPtr != NULL)
        {
            void** (*dataPtrPtr) = vDeqPtr->headPtr->dataPtr;  // point to data of the head
            free(vDeqPtr->headPtr->dataPtr);
            
            if (vDeqPtr->headPtr != NULL)
            {
                int i = vDeqPopHead(vDeqPtr, dataPtrPtr);
            }
        }
        free(vDeqPtr);
    }

    return SUCCESS;
}
