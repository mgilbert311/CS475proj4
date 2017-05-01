# proj4
Xinu Project 4 for OS 475 Evan, Mark, Hunter

Test case is in main_dl.c:
In our test case we have 4 worker threads and 4 different locks. 
Worker 0 requires lock 0 first, and lock 1 second. 
Worker 1 requires lock 2 first, and lock 3 second. 
Worker 2 requires lock 0 first, and lock 1 second. 
Finally, Worker 3 requires lock 3 first, and  lock 2 second. 

What is expected to happen without Worker 3 is as follows: 
Worker 0 acquires lock 0 and does work. 
Worker 1 acquires lock 2 and does work. 
Worker 1 acquires lock 3 and does work. 
Worker 0 acquires lock 1 and does work. 
Worker 1 releases lock 2. 
Worker 1 releases lock 3. 
Worker 0 releases lock 1. 
Worker 0 releases lock 0.
Worker 2 acquires lock 2 and does work.
Worker 2 acquires lock 1 and does work.
Worker 2 releases lock 1.
Worker 2 releases lock 2.

When we add worker 3 a deadlock occurs:
Worker 0 acquires lock 0 and does work.
Worker 1 acquires lock 2 and does work.
Worker 3 acquires lock 3 and does work.
Worker 0 acquires lock 1 and does work.
Worker 0 releases lock 1.
Worker 0 releases lock 0.
Worker 2 acquires lock 0 and does work.
Worker 2 acquires lock 1 and does work.
Worker 2 releases lock 1.
Worker 2 releases lock 0.
Since Worker 3 is holding onto lock 2 and Worker 1 is holding onto lock 3, no work can be done for either process and a deadlock occurs.

With deadlock detection, when Worker 3 grabs lock 3, the process is killed in order for process 1 to complete.



