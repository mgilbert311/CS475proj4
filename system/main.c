/*  main.c  - main */
#include <xinu.h>
#define N 3 //number of philosophers and forks, should be arbitary
#define RAND_MAX 100

lid32 talkLock;// lock used for when the philosopher "talks" (essentally printLock)
lid32 nLeft; // "new" left fork semaphore 
lid32 nRight;// "new" right fork semaphore
lid32 forks[N]; // array of lock id's of size N

/**
 * Delay for a random amount of time
 * @param alpha delay factor
 */
void    holdup(int32 alpha)
{
    long delay = rand() * alpha;
    while (delay-- > 0)
        ;   //no op
}
/**
 * Eat for a random amount of time
 */
void    eat()
{
    holdup(10000);
    //Unlock when we are done?
}
/**
 * Think for a random amount of time
 */
void    think()
{
    holdup(1000);
}
/**
 * Philosopher's code
 * @param phil_id philosopher's id
 */
void    philosopher(uint32 phil_id)
{
    talkLock = lock_create();// create a new lock when philosphers talk
    //Locations of the fork for each philosopher
    uint32 left = phil_id;// left fork
    uint32 right = (phil_id + N-1)%N;//right fork 
    srand(phil_id); //Seed the random number generator with the phil_id
    while (TRUE)
    {
        if((rand() % RAND_MAX) > 30 ){   //think 70% of the time
            acquire(talkLock);//acquire talk lock so ctsw doesnt occur
            kprintf("Philosopher %u thinking: zzzzzzz \n", phil_id);
            release(talkLock);//unlock talklock
            think();
        }else{      //eat 30% of the time
                //acquire the left fork if it is available
                acquire(forks[left]);
                //acquire the right fork if it is available
                acquire(forks[right]);
                //Lock the talk lock so that a cxsw does not happen in the middle
                acquire(talkLock);
                kprintf("Philosopher %u eating: MMMMMMM \n", phil_id);
                release(talkLock);//unlock the talk lock
                eat();
                //Release the forks
                release(forks[left]);
                release(forks[right]);
        }
    }
}
int main(uint32 argc, uint32 *argv)
{
    //initializes new locks to be used in the forks array
    int i;
    for(i = 0; i< N; i++){
        lid32 newLock = lock_create();
        forks[i] = newLock;
    }
    //do not change
    ready(create((void*) philosopher, INITSTK, 15, "Ph1", 1, 0), FALSE);
    ready(create((void*) philosopher, INITSTK, 15, "Ph2", 1, 1), FALSE);
    ready(create((void*) philosopher, INITSTK, 15, "Ph3", 1, 2), FALSE);
    ready(create((void*) philosopher, INITSTK, 15, "Ph4", 1, 3), FALSE);
    ready(create((void*) philosopher, INITSTK, 15, "Ph5", 1, 4), FALSE);
    return 0;
}