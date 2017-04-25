#include <xinu.h>

/**
 * Enter a critical section guarded by lock
 * @param lock	pointer to a mutex lock
 */
void mutex_lock(mutex_t *lock)
{
	//busy-wait loop using test_and_set
	while(test_and_set(lock) == 1);
}


/**
 * Leave a critical section guarded by lock
 * @param lock	pointer to a mutex lock
 */
void mutex_unlock(mutex_t *lock)
{
	*lock = 0;
}
