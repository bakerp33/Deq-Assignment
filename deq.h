#include <stddef.h>

// error codes
#define SUCCESS				(100)
#define BAD_INIT_PARAM		(101)
#define OUT_OF_MEMORY		(102)
#define NULL_PARAM			(103)
#define BAD_DEQ				(104)
#define BAD_POP_DATA_PARAM	(105)
#define BAD_PEEK_DATA_PARAM	(106)
#define POP_EMPTY_DEQ		(107)
#define PEEK_EMPTY_DEQ		(108)


/* typedef for the node struct used in VoidDeq. */
typedef struct VoidDeqNode{
	struct VoidDeqNode* prevPtr;
	struct VoidDeqNode* nextPtr;
	void* dataPtr;
} VoidDeqNode;


/* typedef for the VoidDeq struct itself. */
typedef struct VoidDeq{
	VoidDeqNode* headPtr;
	VoidDeqNode* tailPtr;
} VoidDeq;


/* Initialize a new VoidDeq and record its pointer into (*vDeqPtrPtr).
		vDeqPtrPtr must not be NULL
		(*vDeqPtrPtr) must be NULL */
int vDeqInit(VoidDeq** vDeqPtrPtr);


/* Push 'dataPtr' onto head of 'vDeqPtr'.
		vDeqPtr must not be NULL
		dataPtr can be any value (including NULL) */
int vDeqPushHead(VoidDeq* vDeqPtr, void* dataPtr);


/* Push 'dataPtr' onto tail of 'vDeqPtr'.
		vDeqPtr must not be NULL
		dataPtr can be any value (including NULL) */
int vDeqPushTail(VoidDeq* vDeqPtr, void* dataPtr);


/* Pop the head off 'vDeqPtr' and record in (*dataPtrPtr).
		vDeqPtr must not be NULL
		dataPtrPtr must not be NULL
		(*dataPtrPtr) must be NULL */
int vDeqPopHead(VoidDeq* vDeqPtr, void** dataPtrPtr);


/* Pop the tail off 'vDeqPtr' and record in (*dataPtrPtr).
		vDeqPtr must not be NULL
		dataPtrPtr must not be NULL
		(*dataPtrPtr) must be NULL */
int vDeqPopTail(VoidDeq* vDeqPtr, void** dataPtrPtr);


/* Peek the head of 'vDeqPtr' and record in (*dataPtrPtr).
		vDeqPtr must not be NULL
		dataPtrPtr must not be NULL
		(*dataPtrPtr) must be NULL */
int vDeqPeekHead(VoidDeq* vDeqPtr, void** dataPtrPtr);


/* Peek the tail of 'vDeqPtr' and record in (*dataPtrPtr).
		vDeqPtr must not be NULL
		dataPtrPtr must not be NULL
		(*dataPtrPtr) must be NULL */
int vDeqPeekTail(VoidDeq* vDeqPtr, void** dataPtrPtr);


/* Free all memory used by 'vDeqPtr'. If 'freeDatas' == true,
   then also free all data stored in 'vDeqPtr'.
		vDeqPtr must not be NULL
		freeDatas can be anything */
int vDeqFree(VoidDeq* vDeqPtr, int freeDatas);


/* We define this function in 'deq_tester.c'.
   Always use this function to request memory.
   Use it, but don't define it. */
void* myMalloc(size_t size);
