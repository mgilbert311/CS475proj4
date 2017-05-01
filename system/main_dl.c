/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5 //originally 2

lid32	printer_lock;
lid32	mylock[N];


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
	if(id == 0){
		//acquire lock 0
		acquire(mylock[0]);
		kprintf("worker 0 has lock 0\n");
		//print something
		work(id);
		//acquire lock 1
		acquire(mylock[1]);
		kprintf("worker 0 has lock 1\n");
		//print something
		work(id);
		//release lock 1
		kprintf("worker 0 is about to release lock 1\n");
		release(mylock[1]);
		//release lock 0
		kprintf("worker 0 is about to release lock 0\n");
		release(mylock[0]);
	}else if(id == 1){
		//acquire lock 2
		acquire(mylock[2]);
		kprintf("worker 1 has lock 2\n");
		//print something
		work(id);
		//acquire lock 1
		acquire(mylock[3]);
		kprintf("worker 1 has lock 3\n");
		//print something
		work(id);
		//release lock 1
		kprintf("worker 1 is about to release lock 2\n");
		release(mylock[2]);
		//release lock 2
		kprintf("worker 1 is about to release lock 3\n");
		release(mylock[3]);
	}else if(id==2){//id == 2
		//acquire lock 2
		acquire(mylock[0]);
		kprintf("worker 2 has lock 0\n");
		//print something
		work(id);
		//acquire lock 1
		acquire(mylock[1]);
		kprintf("worker 2 has lock 1\n");
		//print something
		work(id);
		//release lock 1
		kprintf("worker 2 is about to release lock 1\n");
		release(mylock[1]);
		//release lock 0
		kprintf("worker 2 is about to release lock 0\n");
		release(mylock[0]);
	 }else{//id == 3
		//acquire lock 3
		acquire(mylock[3]);
		kprintf("worker 3 has lock 3\n");
		//print something
		work(id);
		//acquire lock 2
		acquire(mylock[2]);
		kprintf("worker 3 has lock 2\n");
		//print something
		work(id);
		//release lock 2
		kprintf("worker 3 is about to release lock 2\n");
		release(mylock[2]);
		//release lock 3
		kprintf("worker 3 is about to release lock 3\n");
		release(mylock[0]);
	}

	// }
	// if (id == 0)
	// {
	// 	acquire(mylock[0]);
	// 	work(id);
	// 	acquire(mylock[1]);
	// 	work(id);
	// 	release(mylock[1]);
	// 	release(mylock[0]);
	// }
	// else
	// {
	// 	acquire(mylock[1]);
	// 	work(id);
	// 	acquire(mylock[0]);
	// 	work(id);
	// 	release(mylock[0]);
	// 	release(mylock[1]);
	// }
}

int	main(uint32 argc, uint32 *argv)
{
	int i;
	printer_lock = lock_create();
	for (i=0; i<N; i++)
		mylock[i] = lock_create();

	ready(create((void*) worker, INITSTK, 15, "Worker 0", 1, 0), FALSE);
	ready(create((void*) worker, INITSTK, 15, "Worker 1", 1, 1), FALSE);
	ready(create((void*) worker, INITSTK, 15, "Worker 1", 1, 2), FALSE);
	ready(create((void*) worker, INITSTK, 15, "Worker 1", 1, 3), FALSE);
	ready(create((void*) worker, INITSTK, 15, "Worker 1", 1, 4), FALSE);

	return 0;
}
