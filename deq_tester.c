#include <stdio.h>
#include <stdlib.h>
#include "deq.h"

#define	WIN  0
#define	FAIL 9999

/* A function (and global) that can trigger malloc fails on demand. */
int mallocFail = 0;
void* myMalloc(size_t size) {
	if (mallocFail) { return NULL; }
	else { return malloc(size); }
}

int test_vDeqInit();
int test_vDeqPushHead();
int test_vDeqPushTail();
int test_vDeqPopHead();
int test_vDeqPopTail();
int test_vDeqPeekHead();
int test_vDeqPeekTail();
int test_vDeqFree();

int main() {
	if (test_vDeqInit() != 0) {
		printf("Quitting testing: 'vDeqInit' failed.\n");
		return 0;
	}
	if (test_vDeqPushHead() != 0) {
		printf("Quitting testing: 'vDeqPushHead' failed.\n");
		return 0;
	}
	if (test_vDeqPushTail() != 0) {
		printf("Quitting testing: 'vDeqPushTail' failed.\n");
		return 0;
	}
	if (test_vDeqPopHead() != 0) {
		printf("Quitting testing: 'vDeqPopHead' failed.\n");
		return 0;
	}
	if (test_vDeqPopTail() != 0) {
		printf("Quitting testing: 'vDeqPopTail' failed.\n");
		return 0;
	}
	if (test_vDeqPeekHead() != 0) {
		printf("Quitting testing: 'vDeqPeekHead' failed.\n");
		return 0;
	}
	if (test_vDeqPeekTail() != 0) {
		printf("Quitting testing: 'vDeqPeekTail' failed.\n");
		return 0;
	}
	if (test_vDeqFree() != 0) {
		printf("Quitting testing: 'vDeqFree' failed.\n");
		return 0;
	}
	return 0;
}

/* Test 'vDeqInit'.
** This function has a memory leak (deq is never free'd) but we
** haven't tested 'vDeqFree' yet, so can't exactly use it here. */
int test_vDeqInit() {
	printf("Testing vDeqInit...\n");

	// test for NULL_PARAM check
	int rVal = vDeqInit(NULL);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'vDeqPtrPtr' is not NULL [likely segfaulted]\n"); return FAIL; }

	// BAD_INIT_PARAM
	int thing = 71;
	VoidDeq* vDeqPtr = (VoidDeq*) (&thing);
	VoidDeq** vDeqPtrPtr = (&vDeqPtr);
	rVal = vDeqInit(vDeqPtrPtr);
	if (rVal != BAD_INIT_PARAM) { printf("  failed: should make sure '(*vDeqPtrPtr)' is NULL\n"); return FAIL; }

	// OUT_OF_MEMORY
	mallocFail = 1;
	vDeqPtr = NULL;
	vDeqPtrPtr = (&vDeqPtr);
	rVal = vDeqInit(vDeqPtrPtr);
	if (rVal != OUT_OF_MEMORY) { printf("  failed: should always check return from malloc (myMalloc) [likely segfaulted]\n"); return FAIL; }
	mallocFail = 0;

	// SUCCESS
	vDeqPtr = NULL;
	vDeqPtrPtr = (&vDeqPtr);
	rVal = vDeqInit(vDeqPtrPtr);
	if (vDeqPtr->headPtr != NULL) { printf("  failed: 'vDeqPtr->headPtr' was not set to NULL\n"); return FAIL; }
	if (vDeqPtr->tailPtr != NULL) { printf("  failed: 'vDeqPtr->tailPtr' was not set to NULL\n"); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed: did not return SUCCESS: %d\n", rVal); return FAIL; }
	else /* (rVal == SUCCESS) */ { printf("  succeeded\n"); return WIN; }
}

/* Test 'vDeqPushHead'.
** This function has a memory leak (deq is never free'd) but we
** haven't tested 'vDeqFree' yet, so can't exactly use it here. */
int test_vDeqPushHead() {
	printf("Testing vDeqPushHead...\n");

	// initialize a new deq to test on
	VoidDeq* vDeqPtr = NULL;
	VoidDeq** vDeqPtrPtr = (&vDeqPtr);
	int rVal = vDeqInit(vDeqPtrPtr);
	if (rVal != SUCCESS) { printf("  initialize 1 (vDeqInit) failed: %d\n", rVal); return FAIL; }

	// NULL_PARAM 1 [should fail]
	int thing = 88;
	rVal = vDeqPushHead(NULL, (&thing));
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'vDeqPtr' is not NULL [likely segfaulted]\n"); return FAIL; }

	// OUT_OF_MEMORY
	mallocFail = 1;
	rVal = vDeqPushHead(vDeqPtr, NULL);
	if (rVal != OUT_OF_MEMORY) { printf("  failed: should always check return from malloc (myMalloc) [likely segfaulted]\n"); return FAIL; }
	mallocFail = 0;

	// NULL_PARAM 2 [okay - push a single item onto empty deq]
	rVal = vDeqPushHead(vDeqPtr, NULL);	// push NULL onto empty deq
	if (vDeqPtr->headPtr == NULL) { printf("  'vDeqPtr->headPtr' should not be NULL after push\n"); return FAIL; }
	if (vDeqPtr->tailPtr == NULL) { printf("  'vDeqPtr->tailPtr' should not be NULL after push\n"); return FAIL; }
	if (vDeqPtr->headPtr != vDeqPtr->tailPtr) { printf("  'vDeqPtr->headPtr' should equal 'vDeqPtr->tailPtr' after push onto empty deq\n"); return FAIL; }
	if (vDeqPtr->headPtr->prevPtr != NULL) { printf("  'vDeqPtr->headPtr->prevPtr' should be NULL after any push\n"); return FAIL; }
	if (vDeqPtr->headPtr->nextPtr != NULL) { printf("  'vDeqPtr->headPtr->nextPtr' should be NULL after push onto empty deq\n"); return FAIL; }
	if (vDeqPtr->headPtr->dataPtr != NULL) { printf("  'vDeqPtr->headPtr->dataPtr' should be NULL after pushHead NULL\n"); return FAIL; }
	if (rVal != SUCCESS) { printf("  vDeqPushHead did not return SUCCESS: %d\n", rVal); return FAIL; }

	// test a second pushHeads (push onto non-empty deq)
	// initialize a new deq to test on
	vDeqPtr = NULL;		// this introduces a memory leak - no 'free' tested yet
	vDeqPtrPtr = (&vDeqPtr);
	rVal = vDeqInit(vDeqPtrPtr);	// should be safe, but could have run out of memory
	if (rVal != SUCCESS) { printf("  initialize 2 (vDeqInit) failed: %d\n", rVal); return FAIL; }

	// push first item and grab it's Node
	int v1 = (0x0000000F & rand()) | (0x00000001);
	vDeqPushHead(vDeqPtr, (&v1));	// will work (because tests above passed)
	VoidDeqNode* node1 = vDeqPtr->headPtr;
	// then push another one - this is what we're testing now
	int v2 = (0x000000F0 & rand()) | (0x00000010);
	rVal = vDeqPushHead(vDeqPtr, (&v2));	// this is what we're testing now

	// see what we've got after inserting two values
	if (vDeqPtr->headPtr == NULL) { printf("  failed: 'vDeqPtr->headPtr' should not be NULL\n"); return FAIL; }
	if (vDeqPtr->tailPtr == NULL) { printf("  failed: 'vDeqPtr->tailPtr' should not be NULL\n"); return FAIL; }
	if (vDeqPtr->headPtr == vDeqPtr->tailPtr) { printf("  failed: 'vDeqPtr->headPtr' should not equal 'vDeqPtr->tailPtr'\n"); return FAIL; }
	if (vDeqPtr->tailPtr != node1) { printf("  failed: 'vDeqPtr->tailPtr' should be first push Node\n"); return FAIL; }
	if (vDeqPtr->tailPtr->prevPtr != vDeqPtr->headPtr) { printf("  failed: 'vDeqPtr->tailPtr->prevPtr' should equal node2\n"); return FAIL; }
	if (vDeqPtr->tailPtr->nextPtr != NULL) { printf("  failed: 'vDeqPtr->tailPtr->nextPtr' should equal NULL\n"); return FAIL; }
	if (vDeqPtr->tailPtr->dataPtr != (&v1)) { printf("  failed: 'vDeqPtr->tailPtr->dataPtr' did not record data parameter\n"); return FAIL; }
	if (vDeqPtr->headPtr->prevPtr != NULL) { printf("  failed: 'vDeqPtr->headPtr->prevPtr' should equal NULL\n"); return FAIL; }
	if (vDeqPtr->headPtr->nextPtr != vDeqPtr->tailPtr) { printf("  failed: 'vDeqPtr->headPtr->nextPtr' should equal vDeqPtr->tailPtr\n"); return FAIL; }
	if (vDeqPtr->headPtr->dataPtr != (&v2)) { printf("  failed: 'vDeqPtr->headPtr->dataPtr' did not record data parameter\n"); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed: did not return SUCCESS: %d\n", rVal); return FAIL; }
	else /* (rVal == SUCCESS) */ { printf("  succeeded\n"); return WIN; }
}

/* Test 'vDeqPushTail'.
** This function has a memory leak (deq is never free'd) but we
** haven't tested 'vDeqFree' yet, so can't exactly use it here. */
int test_vDeqPushTail() {
	printf("Testing vDeqPushTail...\n");

	// initialize a new deq to test on
	VoidDeq* vDeqPtr = NULL;
	VoidDeq** vDeqPtrPtr = (&vDeqPtr);
	int rVal = vDeqInit(vDeqPtrPtr);
	if (rVal != SUCCESS) { printf("  initialize 1 (vDeqInit) failed: %d\n", rVal); return FAIL; }

	// NULL_PARAM 1 [should fail]
	int thing = 99;
	rVal = vDeqPushTail(NULL, (&thing));
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'vDeqPtr' is not NULL [likely segfaulted]\n"); return FAIL; }

	// OUT_OF_MEMORY
	mallocFail = 1;
	rVal = vDeqPushTail(vDeqPtr, NULL);
	if (rVal != OUT_OF_MEMORY) { printf("  failed: should always check return from malloc (myMalloc) [likely segfaulted]\n"); return FAIL; }
	mallocFail = 0;

	// NULL_PARAM 2 [okay - push a single item onto empty deq]
	rVal = vDeqPushTail(vDeqPtr, NULL);	// push NULL onto empty deq
	if (vDeqPtr->headPtr == NULL) { printf("  'vDeqPtr->headPtr' should not be NULL after push\n"); return FAIL; }
	if (vDeqPtr->tailPtr == NULL) { printf("  'vDeqPtr->tailPtr' should not be NULL after push\n"); return FAIL; }
	if (vDeqPtr->headPtr != vDeqPtr->tailPtr) { printf("  'vDeqPtr->headPtr' should equal 'vDeqPtr->tailPtr' after push onto empty deq\n"); return FAIL; }
	if (vDeqPtr->tailPtr->prevPtr != NULL) { printf("  'vDeqPtr->tailPtr->prevPtr' should be NULL after any push\n"); return FAIL; }
	if (vDeqPtr->tailPtr->nextPtr != NULL) { printf("  'vDeqPtr->tailPtr->nextPtr' should be NULL after push onto empty deq\n"); return FAIL; }
	if (vDeqPtr->tailPtr->dataPtr != NULL) { printf("  'vDeqPtr->tailPtr->dataPtr' should be NULL after pushTail NULL\n"); return FAIL; }
	if (rVal != SUCCESS) { printf("  vDeqPushTail did not return SUCCESS: %d\n", rVal); return FAIL; }

	// test a second pushTail (push onto non-empty deq)
	// initialize a new deq to test on
	vDeqPtr = NULL;		// this introduces a memory leak - no 'free' tested yet
	vDeqPtrPtr = (&vDeqPtr);
	rVal = vDeqInit(vDeqPtrPtr);	// should be safe, but could have run out of memory
	if (rVal != SUCCESS) { printf("  initialize 2 (vDeqInit) failed: %d\n", rVal); return FAIL; }

	// push first item and grab it's Node
	int v1 = (0x0000000F & rand()) | (0x00000001);
	vDeqPushTail(vDeqPtr, (&v1));	// will work (because tests above passed)
	VoidDeqNode* node1 = vDeqPtr->tailPtr;
	// then push another one - this is what we're testing now
	int v2 = (0x000000F0 & rand()) | (0x00000010);
	rVal = vDeqPushTail(vDeqPtr, (&v2));	// this is what we're testing now

	// see what we've got after inserting two values
	if (vDeqPtr->headPtr == NULL) { printf("  failed: 'vDeqPtr->headPtr' should not be NULL\n"); return FAIL; }
	if (vDeqPtr->tailPtr == NULL) { printf("  failed: 'vDeqPtr->tailPtr' should not be NULL\n"); return FAIL; }
	if (vDeqPtr->headPtr == vDeqPtr->tailPtr) { printf("  failed: 'vDeqPtr->headPtr' should not equal 'vDeqPtr->tailPtr'\n"); return FAIL; }
	if (vDeqPtr->headPtr != node1) { printf("  failed: 'vDeqPtr->headPtr' should be first push Node\n"); return FAIL; }
	if (vDeqPtr->tailPtr->prevPtr != vDeqPtr->headPtr) { printf("  failed: 'vDeqPtr->tailPtr->prevPtr' should equal node2\n"); return FAIL; }
	if (vDeqPtr->tailPtr->nextPtr != NULL) { printf("  failed: 'vDeqPtr->tailPtr->nextPtr' should equal NULL\n"); return FAIL; }
	if (vDeqPtr->tailPtr->dataPtr != (&v2)) { printf("  failed: 'vDeqPtr->tailPtr->dataPtr' did not record data parameter\n"); return FAIL; }
	if (vDeqPtr->headPtr->prevPtr != NULL) { printf("  failed: 'vDeqPtr->headPtr->prevPtr' should equal NULL\n"); return FAIL; }
	if (vDeqPtr->headPtr->nextPtr != vDeqPtr->tailPtr) { printf("  failed: 'vDeqPtr->headPtr->nextPtr' should equal vDeqPtr->tailPtr\n"); return FAIL; }
	if (vDeqPtr->headPtr->dataPtr != (&v1)) { printf("  failed: 'vDeqPtr->headPtr->dataPtr' did not record data parameter\n"); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed: did not return SUCCESS: %d\n", rVal); return FAIL; }
	else /* (rVal == SUCCESS) */ { printf("  succeeded\n"); return WIN; }
}

/* Test 'vDeqPopHead'.
** This function has a memory leak (deq is never free'd) but we
** haven't tested 'vDeqFree' yet, so can't exactly use it here. */
int test_vDeqPopHead() {
	printf("Testing vDeqPopHead...\n");

	// initialize a new deq to test on
	VoidDeq* vDeqPtr = NULL;
	VoidDeq** vDeqPtrPtr = (&vDeqPtr);
	int rVal = vDeqInit(vDeqPtrPtr);
	if (rVal != SUCCESS) { printf("  initialize 1 (vDeqInit) failed: %d\n", rVal); return FAIL; }

	// test NULL_PARAMs
	int thing = 37;
	void* dataPtr = (void*) (&thing);
	void** dataPtrPtr = (&dataPtr);
	rVal = vDeqPopHead(NULL, dataPtrPtr);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'vDeqPtr' is not NULL [likely segfaulted]\n"); return FAIL; }
	rVal = vDeqPopHead(vDeqPtr, NULL);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'dataPtrPtr' is not NULL [likely segfaulted]\n"); return FAIL; }
	rVal = vDeqPopHead(vDeqPtr, dataPtrPtr);
	if (rVal != BAD_POP_DATA_PARAM) { printf("  failed: should make sure '(*dataPtrPtr)' is NULL\n"); return FAIL; }

	// test pop from empty deq
	dataPtr = NULL;
	rVal = vDeqPopHead(vDeqPtr, dataPtrPtr);
	if (rVal != POP_EMPTY_DEQ) { printf("  failed: should check for empty deq before any pop\n"); return FAIL; }

	// push one item
	int thing1 = 0x000000FF & rand();
	rVal = vDeqPushHead(vDeqPtr, (&thing1));
	if (rVal != SUCCESS) { printf("  initialize 2 (vDeqPushHead) failed: %d\n", rVal); return FAIL; }

	// push another item
	int thing2 = 0x00000FF0 & rand();
	dataPtr = NULL;
	rVal = vDeqPushHead(vDeqPtr, (&thing2));
	if (rVal != SUCCESS) { printf("  initialize 3 (vDeqPushHead) failed: %d\n", rVal); return FAIL; }

	// pop off the head
	rVal = vDeqPopHead(vDeqPtr, dataPtrPtr);
	if (vDeqPtr->headPtr == NULL) { printf("  'vDeqPtr->headPtr' should not be NULL after pop when n>1\n"); return FAIL; }
	if (vDeqPtr->tailPtr == NULL) { printf("  'vDeqPtr->tailPtr' should not be NULL after pop when n>1\n"); return FAIL; }
	if (vDeqPtr->headPtr != vDeqPtr->tailPtr) { printf("  'vDeqPtr->headPtr' should equal 'vDeqPtr->tailPtr' after pop when n=2\n"); return FAIL; }
	if (vDeqPtr->headPtr->prevPtr != NULL) { printf("  'vDeqPtr->headPtr->prevPtr' should be NULL after pop when n=2\n"); return FAIL; }
	if (vDeqPtr->headPtr->nextPtr != NULL) { printf("  'vDeqPtr->headPtr->nextPtr' should be NULL after pop when n=2\n"); return FAIL; }
	if (vDeqPtr->headPtr->dataPtr != (&thing1)) { printf("  'vDeqPtr->headPtr->dataPtr' should be %d after popping %d\n", thing1, thing2); return FAIL; }
	if ((*dataPtrPtr) != (&thing2)) { printf("  'dataPtrPtr' should be %d after popping %d\n", thing2, thing2); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed: did not return SUCCESS\n"); return FAIL; }

	// pop to make an empty deq
	dataPtr = NULL;
	rVal = vDeqPopHead(vDeqPtr, dataPtrPtr);
	if (vDeqPtr->headPtr != NULL) { printf("  'vDeqPtr->headPtr' should be NULL after pop when n=1\n"); return FAIL; }
	if (vDeqPtr->tailPtr != NULL) { printf("  'vDeqPtr->tailPtr' should be NULL after pop when n=1\n"); return FAIL; }
	if ((*dataPtrPtr) != (&thing1)) { printf("  'dataPtrPtr' should be %d after popping %d\n", thing1, thing1); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed: did not return SUCCESS: %d\n", rVal); return FAIL; }
	else /* (rVal == SUCCESS) */ { printf("  succeeded\n"); return WIN; }
}

/* Test 'vDeqPopTail'.
** This function has a memory leak (deq is never free'd) but we
** haven't tested 'vDeqFree' yet, so can't exactly use it here. */
int test_vDeqPopTail() {
	printf("Testing vDeqPopTail...\n");

	// initialize a new deq to test on
	VoidDeq* vDeqPtr = NULL;
	VoidDeq** vDeqPtrPtr = (&vDeqPtr);
	int rVal = vDeqInit(vDeqPtrPtr);
	if (rVal != SUCCESS) { printf("  initialize 1 (vDeqInit) failed: %d\n", rVal); return FAIL; }

	// test NULL_PARAMs
	int thing = 37;
	void* dataPtr = (void*) (&thing);
	void** dataPtrPtr = (&dataPtr);
	rVal = vDeqPopTail(NULL, dataPtrPtr);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'vDeqPtr' is not NULL [likely segfaulted]\n"); return FAIL; }
	rVal = vDeqPopTail(vDeqPtr, NULL);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'dataPtrPtr' is not NULL [likely segfaulted]\n"); return FAIL; }
	rVal = vDeqPopTail(vDeqPtr, dataPtrPtr);
	if (rVal != BAD_POP_DATA_PARAM) { printf("  failed: should make sure '(*dataPtrPtr)' is NULL\n"); return FAIL; }

	// test pop from empty deq
	dataPtr = NULL;
	rVal = vDeqPopTail(vDeqPtr, dataPtrPtr);
	if (rVal != POP_EMPTY_DEQ) { printf("  failed: should check for empty deq before any pop\n"); return FAIL; }

	// push one item
	int thing1 = 0x000000FF & rand();
	rVal = vDeqPushTail(vDeqPtr, (&thing1));
	if (rVal != SUCCESS) { printf("  initialize 2 (vDeqPushTail) failed: %d\n", rVal); return FAIL; }

	// push another item
	int thing2 = 0x00000FF0 & rand();
	dataPtr = NULL;
	rVal = vDeqPushTail(vDeqPtr, (&thing2));
	if (rVal != SUCCESS) { printf("  initialize 3 (vDeqPushTail) failed: %d\n", rVal); return FAIL; }

	// pop off the tail
	rVal = vDeqPopTail(vDeqPtr, dataPtrPtr);
	if (vDeqPtr->headPtr == NULL) { printf("  'vDeqPtr->headPtr' should not be NULL after pop when n>1\n"); return FAIL; }
	if (vDeqPtr->tailPtr == NULL) { printf("  'vDeqPtr->tailPtr' should not be NULL after pop when n>1\n"); return FAIL; }
	if (vDeqPtr->headPtr != vDeqPtr->tailPtr) { printf("  'vDeqPtr->headPtr' should equal 'vDeqPtr->tailPtr' after pop when n=2\n"); return FAIL; }
	if (vDeqPtr->headPtr->prevPtr != NULL) { printf("  'vDeqPtr->headPtr->prevPtr' should be NULL after pop when n=2\n"); return FAIL; }
	if (vDeqPtr->headPtr->nextPtr != NULL) { printf("  'vDeqPtr->headPtr->nextPtr' should be NULL after pop when n=2\n"); return FAIL; }
	if (vDeqPtr->headPtr->dataPtr != (&thing1)) { printf("  'vDeqPtr->headPtr->dataPtr' should be %d after popping %d\n", thing1, thing2); return FAIL; }
	if ((*dataPtrPtr) != (&thing2)) { printf("  'dataPtrPtr' should be %d after popping %d\n", thing2, thing2); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed: did not return SUCCESS: %d\n", rVal); return FAIL; }

	// pop to make an empty deq
	dataPtr = NULL;
	rVal = vDeqPopTail(vDeqPtr, dataPtrPtr);
	if (vDeqPtr->headPtr != NULL) { printf("  'vDeqPtr->headPtr' should be NULL after pop when n=1\n"); return FAIL; }
	if (vDeqPtr->tailPtr != NULL) { printf("  'vDeqPtr->tailPtr' should be NULL after pop when n=1\n"); return FAIL; }
	if ((*dataPtrPtr) != (&thing1)) { printf("  'dataPtrPtr' should be %d after popping %d\n", thing1, thing1); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed: did not return SUCCESS: %d\n", rVal); return FAIL; }
	else /* (rVal == SUCCESS) */ { printf("  succeeded\n"); return WIN; }
}

/* Test 'vDeqPeekHead'.
** This function has a memory leak (deq is never free'd) but we
** haven't tested 'vDeqFree' yet, so can't exactly use it here. */
int test_vDeqPeekHead() {
	printf("Testing vDeqPeekHead...\n");

	// initialize a new deq to test on
	VoidDeq* vDeqPtr = NULL;
	VoidDeq** vDeqPtrPtr = (&vDeqPtr);
	int rVal = vDeqInit(vDeqPtrPtr);
	if (rVal != SUCCESS) { printf("  initialize 1 (vDeqInit) failed: %d\n", rVal); return FAIL; }

	// test NULL_PARAMs
	int thing = 37;
	void* dataPtr = (void*) (&thing);
	void** dataPtrPtr = (&dataPtr);
	rVal = vDeqPeekHead(NULL, dataPtrPtr);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'vDeqPtr' is not NULL [likely segfaulted]\n"); return FAIL; }
	rVal = vDeqPeekHead(vDeqPtr, NULL);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'dataPtrPtr' is not NULL [likely segfaulted]\n"); return FAIL; }
	rVal = vDeqPeekHead(vDeqPtr, dataPtrPtr);
	if (rVal != BAD_PEEK_DATA_PARAM) { printf("  failed: should make sure '(*dataPtrPtr)' is NULL\n"); return FAIL; }

	// test peek from empty deq
	dataPtr = NULL;
	rVal = vDeqPeekHead(vDeqPtr, dataPtrPtr);
	if (rVal != PEEK_EMPTY_DEQ) { printf("  failed: should check for empty deq before any peek\n"); return FAIL; }

	// push one item
	int thing1 = 0x000000FF & rand();
	rVal = vDeqPushHead(vDeqPtr, (&thing1));
	if (rVal != SUCCESS) { printf("  initialize 2 (vDeqPushHead) failed: %d\n", rVal); return FAIL; }

	// peek that item
	dataPtr = NULL;
	rVal = vDeqPeekHead(vDeqPtr, dataPtrPtr);
	if ((*dataPtrPtr) != (&thing1)) { printf("  'dataPtrPtr' should be %d after pushing %d\n", thing1, thing1); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed: did not return SUCCESS: %d\n", rVal); return FAIL; }

	// push another item
	int thing2 = 0x00000FF0 & rand();
	dataPtr = NULL;
	rVal = vDeqPushHead(vDeqPtr, (&thing2));
	if (rVal != SUCCESS) { printf("  initialize 3 (vDeqPushHead) failed: %d\n", rVal); return FAIL; }

	// peek the second item
	dataPtr = NULL;
	rVal = vDeqPeekHead(vDeqPtr, dataPtrPtr);
	if ((*dataPtrPtr) != (&thing2)) { printf("  'dataPtrPtr' should be %d after pushing %d\n", thing2, thing2); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed: did not return SUCCESS: %d\n", rVal); return FAIL; }
	else /* (rVal == SUCCESS) */ { printf("  succeeded\n"); return WIN; }
}

/* Test 'vDeqPeekTail'.
** This function has a memory leak (deq is never free'd) but we
** haven't tested 'vDeqFree' yet, so can't exactly use it here. */
int test_vDeqPeekTail() {
	printf("Testing vDeqPeekTail...\n");

	// initialize a new deq to test on
	VoidDeq* vDeqPtr = NULL;
	VoidDeq** vDeqPtrPtr = (&vDeqPtr);
	int rVal = vDeqInit(vDeqPtrPtr);
	if (rVal != SUCCESS) { printf("  initialize 1 (vDeqInit) failed: %d\n", rVal); return FAIL; }

	// test NULL_PARAMs
	int thing = 37;
	void* dataPtr = (void*) (&thing);
	void** dataPtrPtr = (&dataPtr);
	rVal = vDeqPeekTail(NULL, dataPtrPtr);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'vDeqPtr' is not NULL [likely segfaulted]\n"); return FAIL; }
	rVal = vDeqPeekTail(vDeqPtr, NULL);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'dataPtrPtr' is not NULL [likely segfaulted]\n"); return FAIL; }
	rVal = vDeqPeekTail(vDeqPtr, dataPtrPtr);
	if (rVal != BAD_PEEK_DATA_PARAM) { printf("  failed: should make sure '(*dataPtrPtr)' is NULL\n"); return FAIL; }

	// test peek from empty deq
	dataPtr = NULL;
	rVal = vDeqPeekTail(vDeqPtr, dataPtrPtr);
	if (rVal != PEEK_EMPTY_DEQ) { printf("  failed: should check for empty deq before any peek\n"); return FAIL; }

	// push one item
	int thing1 = 0x000000FF & rand();
	rVal = vDeqPushTail(vDeqPtr, (&thing1));
	if (rVal != SUCCESS) { printf("  initialize 2 (vDeqPushTail) failed: %d\n", rVal); return FAIL; }

	// peek that item
	dataPtr = NULL;
	rVal = vDeqPeekTail(vDeqPtr, dataPtrPtr);
	if ((*dataPtrPtr) != (&thing1)) { printf("  'dataPtrPtr' should be %d after pushing %d\n", thing1, thing1); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed: did not return SUCCESS: %d\n", rVal); return FAIL; }

	// push another item
	int thing2 = 0x00000FF0 & rand();
	dataPtr = NULL;
	rVal = vDeqPushTail(vDeqPtr, (&thing2));
	if (rVal != SUCCESS) { printf("  initialize 3 (vDeqPushTail) failed: %d\n", rVal); return FAIL; }

	// peek the second item
	dataPtr = NULL;
	rVal = vDeqPeekTail(vDeqPtr, dataPtrPtr);
	if ((*dataPtrPtr) != (&thing2)) { printf("  'dataPtrPtr' should be %d after pushing %d\n", thing2, thing2); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed: did not return SUCCESS: %d\n", rVal); return FAIL; }
	else /* (rVal == SUCCESS) */ { printf("  succeeded\n"); return WIN; }
}

/* Test 'vDeqPeekTail'.
   Weird tests because I can't verify that 'free' ran - the pointers remain... */
int test_vDeqFree() {
	printf("Testing vDeqFree...\n");

	// test for NULL_PARAM check
	int rVal = vDeqFree(NULL, 0);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'vDeqPtr' is not NULL [likely segfaulted]\n"); return FAIL; }

	// test freeing empty deq + freeDatas
	VoidDeq* vDeqPtr = NULL;
	VoidDeq** vDeqPtrPtr = (&vDeqPtr);
	rVal = vDeqInit(vDeqPtrPtr);
	if (rVal != SUCCESS) { printf("  initialize 1 (vDeqInit) failed: %d\n", rVal); return FAIL; }
	rVal = vDeqFree(vDeqPtr, 1);
	if (rVal != SUCCESS) { printf("  failed 1 (empty deq + freeDatas): with error %d\n", rVal); return FAIL; }

	// test freeing empty deq + !freeDatas
	vDeqPtr = NULL;
	vDeqPtrPtr = (&vDeqPtr);
	rVal = vDeqInit(vDeqPtrPtr);
	if (rVal != SUCCESS) { printf("  initialize 2 (vDeqInit) failed: %d\n", rVal); return FAIL; }
	rVal = vDeqFree(vDeqPtr, 0);
	if (rVal != SUCCESS) { printf("  failed 2 (empty deq + !freeDatas): with error %d\n", rVal); return FAIL; }

	// test freeing !empty deq + freeDatas
	vDeqPtr = NULL;
	vDeqPtrPtr = (&vDeqPtr);
	rVal = vDeqInit(vDeqPtrPtr);
	if (rVal != SUCCESS) { printf("  initialize 3 (vDeqInit) failed: %d\n", rVal); return FAIL; }
	int* v1 = malloc(sizeof(int));
	rVal = vDeqPushHead(vDeqPtr, v1);
	if (rVal != SUCCESS) { printf("  pushHead 3 (vDeqPushHead) failed: %d\n", rVal); return FAIL; }
	rVal = vDeqFree(vDeqPtr, 1);
	if (rVal != SUCCESS) { printf("  failed 3 (!empty deq + freeDatas): %d\n", rVal); return FAIL; }

	// test freeing non-empty deq + !freeDatas
	vDeqPtr = NULL;
	vDeqPtrPtr = (&vDeqPtr);
	rVal = vDeqInit(vDeqPtrPtr);
	if (rVal != SUCCESS) { printf("  initialize 4 (vDeqInit) failed: %d\n", rVal); return FAIL; }
	int v2 = 99;
	rVal = vDeqPushHead(vDeqPtr, (&v2));
	if (rVal != SUCCESS) { printf("  pushHead 4 (vDeqPushHead) failed: %d\n", rVal); return FAIL; }
	rVal = vDeqFree(vDeqPtr, 0);
	if (rVal != SUCCESS) { printf("  failed 4 (!empty deq + !freeDatas): %d\n", rVal); return FAIL; }
	else /* (rVal == SUCCESS) */ { printf("  succeeded\n"); return WIN; }
}
