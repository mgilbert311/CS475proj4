/*  main.c  - main */
#include <xinu.h>
#define N 3 //number of philosophers and forks, should be arbitary
#define RAND_MAX 100
//Forks are initialized to 0 (open)
//mutex_t forks[N] = {};
//Lock to make sure that the print statement isn't interrupted 
//mutex_t talk = FALSE;
// create all the new locks
//lid32 nForks[N]; // 0 is left 1 is right
lid32 talkLock;
lid32 nLeft; // "new" left fork semaphore 
lid32 nRight;// "new" right fork semaphore
lid32 forks[N];
//lid32 forks[N];
//lid32 nTalk = lock_create();// "new" talk semaphore 
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
    //lid32 lFork = (lid32)phil_id;
    //lid32 rFork = (lid32)phil_id+1;
    talkLock = lock_create();
    //nLeft = lock_create();
    //nRight = lock_create();
    //Locations of the fork for each philosopher
    uint32 left = phil_id;
    uint32 right = (phil_id + N-1)%N;   
    srand(phil_id); //Seed the random number generator with the phil_id
    while (TRUE)
    {
        if((rand() % RAND_MAX) > 30 ){   //think 70% of the time
            acquire(talkLock);
            //mutex_lock(&talk); //May not need this 
            kprintf("Philosopher %u thinking: zzzzzzz \n", phil_id);
            release(talkLock);
            //mutex_unlock(&talk);
            think();
        }else{      //eat 30% of the time
            //acquire the left fork if it is available
            //if(forks[lFork] == 0){//if(forks[left] == 0){ //Lock is open
                //mutex_lock(&forks[left]);
                acquire(forks[left]);
            //}
            //Try to acquire other fork
            //if(forks[rFork] == 1){ 
                //Right fork is taken so drop the left fork
                //mutex_unlock(&forks[left]);
                //release(nLeft);
            //}
            //if(forks[lFork] == 1 && forks[rFork] == 0){ 
                //Make sure the other is available and that the left Fork is taken
                //mutex_lock(&forks[right]);
                acquire(forks[right]);
                //mutex_lock(&talk); //Lock the print so that a cxsw does not happen in the middle
                acquire(talkLock);
                kprintf("Philosopher %u eating: MMMMMMM \n", phil_id); 
                //mutex_unlock(&talk); 
                release(talkLock);
                eat();
                //Release the forks
                //mutex_unlock(&forks[left]);
                release(forks[left]);
                //mutex_unlock(&forks[right]);
                release(forks[right]);
            //}
        }
    }
}
int main(uint32 argc, uint32 *argv)
{
    //initializes new locks to be used
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