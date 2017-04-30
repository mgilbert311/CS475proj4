#include <xinu.h>

local	lid32	newlock(void);

/**
 * Create a new semaphore and return the ID to the caller
 * @return ID of the mutex to caller, or SYSERR
 */
lid32	lock_create()
{
	intmask	mask;		/* saved interrupt mask	*/
	lid32	lockid;		/* lock ID to return	*/

	mask = disable();

	if ((lockid = newlock()) == SYSERR)
	{
		restore(mask);
		return SYSERR;
	}

	restore(mask);
	return lockid;
}

/**
 * Allocate an unused lock and return its index
 * @return	ID of free lock
 */
local	lid32	newlock(void)
{
	lid32	lockid;			/* ID to return	*/
	int32	i;			/* iterate through # entries	*/
	struct lockentry *lockptr;
	
	//Loop through each element in the lock table.
	for(i = 0; i < NLOCK; i++){
		lockptr = &locktab[i];

		//Find a free lock
		if(lockptr->state == LOCK_FREE){

			//Set lock state to used
			lockptr->state = LOCK_USED;
			//Reset the lock mutex to FALSE
			lockptr->lock = FALSE;

			lockid = i;
			return lockid;
		}
	}

	//If there is no such lock, return SYSERR
	return SYSERR; /* no lock found */
}


/**
 * Delete a lock by releasing its table entry
 * @param ID of lock to release
 */
syscall	lock_delete(lid32 lockid)
{
	intmask mask;			/* saved interrupt mask		*/
	struct	lockentry *lptr;	/* ptr to lock table entry	*/

	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}

	lptr = &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}

	//Reset lock's state to free
	lptr -> state = LOCK_FREE;
	//Reset the lock's mutex to FALSE
	lptr -> lock = FALSE;

	//Remove all processes waiting on its queue, and send them to the ready queue
	pid32 tempProc;
	while(!isempty(lptr->wait_queue)){
		tempProc = dequeue(lptr->wait_queue);
		enqueue(tempProc, readyqueue, lockid);
	
	//TODO (RAG) - remove all RAG edges to and from this lock
		// rag_dealloc(tempProc, lockid); 
	}

	resched();
	restore(mask);
	return OK;
}


/**
 * Acquires the given lock, or cause current process to wait until acquired
 * @param lockid	lock on which to wait
 * @return status
 */
syscall	acquire(lid32 lockid)
{
	intmask mask;			// saved interrupt mask
	struct	lockentry *lptr;	// ptr to sempahore table entry

	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}

	kprintf("acquire() lockid: %d\n", lockid);
	lptr = &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}

	//Enqueue the current process ID on the lock's wait queue
	// pid32 currProc = dequeue(readyqueue); @hkurtis what does this line do?
	enqueue(currpid, lptr->wait_queue, lockid);

	//TODO (RAG) - add a request edge in the RAG
	// rag_request(currProc, lockid);

	restore(mask);				//reenable interrupts

	//Lock the mutex!
	lptr -> lock = TRUE;

	mask = disable();			//disable interrupts

	//TODO (RAG) - we reache this point. Must've gotten the lock! Transform request edge to allocation edge
	// rag_alloc(currProc, lockid);

	restore(mask);				//reenable interrupts
	return OK;
}

/**
 * Unlock, releasing a process if one is waiting
 * @param lockid ID of lock to unlock
 * @return status
 */
syscall	release(lid32 lockid)
{
	intmask mask;			/* saved interrupt mask		*/
	struct	lockentry *lptr;	/* ptr to lock table entry	*/

	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}
	lptr= &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}

	//Remove current process' ID from the lock's queue
	pid32 currProc = remove(currpid, lptr -> wait_queue);

	//Unlock the mutex
	lptr -> lock = FALSE;

	//TODO (RAG) - remove allocation edge from RAG
	// rag_dealloc(currProc, lockid);

	restore(mask);
	return OK;
}
