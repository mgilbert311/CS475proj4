/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

#define N 2

lid32	printer_lock;
lid32	lock;


/**
 * Delay for a random amount of time
 * @param alpha delay factor
 */
void	holdup(int32 alpha)
{
	long delay = rand() * alpha;
	while (delay-- > 0)
		;	//no op
}

/**
 * Work for a random amount of time
 * @param id ID of worker
 */
void	work(uint32 id)
{
	acquire(printer_lock);
	kprintf("Worker %d: Buzz buzz buzz\n", id);
	release(printer_lock);
	holdup(10000);
}


/**
 * Worker code
 * @param id ID of worker
 */
void	worker(uint32 id)
{
	if (id == 0)
	{
		acquire(lock);
		work(id);
		while(1);
	}
	else
	{
		acquire(lock);
		work(id);
		while(1);
	}
}

int	main(uint32 argc, uint32 *argv)
{
	int i;
	printer_lock = lock_create();
	lock = lock_create();

	ready(create((void*) worker, INITSTK, 15, "Worker 0", 1, 0), FALSE);
	ready(create((void*) worker, INITSTK, 15, "Worker 1", 1, 1), FALSE);

	return 0;
}
