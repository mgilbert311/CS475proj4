#include <xinu.h>
/**
* Single nstance resource allocation graph (RAG)
* Implemented using matrix representation of the graph
*/
//Using a matrix so lookups/adds are O(1)


//The nodes are in order from l0-l9 and p0-p19
int parent[SIZE]; //Array for nodes involved, indices are important
int color[SIZE]; //Array for which color a node is 
int count = 0; //for backtracking
int dllockid; //the id of a lock involved in deadlock

/*
 * Adds a request edge from pid to lockid
 * @param pid
 * @param lockid
 */
void rag_request(int pid, int lockid){
	//Find the correct cell and initialize
	int cell = translateIndex(pid);
	RAG[cell][lockid] = 1;
}


void rag_alloc(int pid, int lockid){
	//find the correct cell and update the RAG
	int cell = translateIndex(pid);
	RAG[lockid][cell] = 1;
	RAG[cell][lockid] = 0;
}


void rag_dealloc(int pid, int lockid){
	//Find the correct cell and then set its value to 0
	int cell = translateIndex(pid);
	RAG[lockid][cell] = 0;
}


//Print the RAG
void rag_print(){
	int i, j;
	for(i=0; i<SIZE;i++){
		if(i <10) kprintf("l%d", i);
		else kprintf("p%d", i-10);
	}
	kprintf("\n");
	for(i=0; i< SIZE; i++){
		if(i<10) kprintf("l%d   ", i);
		else kprintf("p%d   ", i-10);
		for(j=0; j<SIZE; j++){
			kprintf("%d", RAG[i][j]);
		}
		kprintf("\n"); //New line after each row
	}

}

/**
 * Translate only for a thread node
 *
 */
int translateIndex(int v){
	//If it is a lock node it will occupy cells 0-9
	//If it is a thread node it will occupy cells 10-29
	return v + 10;
}


//DFS
void deadlock_detect(void){
	//kprintf("In detect\n");
	//node parent[SIZE]; //Can max visit all nodes?
	int i;
	//Initialize all to be unvisited
	for(i=0; i<SIZE; i++){
			color[i] = WHITE;
			parent[i] = -1; //-1 as NULL
		}
	
	for(i=0; i<SIZE; i++){
		if(color[i] == WHITE){
			//Visit an unseen node
			deadlock_helper(i);
			}		
		}
	//Found nothing
}

//DFS helper
int deadlock_helper(int index){
	int j;
	color[index] = GREY; //This is the node we just visited

		for(j=0; j<SIZE; j++){
		//Don't worry about non existant edges
		if(RAG[index][j] == 1){
			//In the an adjacent node
			if(color[j] == WHITE){
				parent[j] = index;//Add where the node came from
				deadlock_helper(j); //Recurse
			}else if(color[j] == GREY){
				//Found a cycle/deadlock stop
				parent[j] = index; //Add the last node to the parent
				print_parent(j);
                deadlock_recover();

			}
		}

		}
	color[index] = BLACK;
	return 1; //Did not find anything
}

void print_parent(int index){
	int i; //If count ==1 then we can stop
	kprintf("DEADLOCK    ");
	i = index; //original index

	while(count < 1){ //Seen it twice
		//Get the first parent
		i = parent[i];
		//Print out the associated handle
		if(i < 10){ //print out a lockid
			kprintf("lockid=%d     ", i);
            dllockid = i;
		}else{
			kprintf("pid=%d     ", i-10);
		}

		if(i == index){
			count++; //Break out if we have seen ourselves again
		} 
	}
	kprintf("\n");
}

void deadlock_recover(){

    int i, dlpid;
    for(i=NLOCK; i<NPROC; i++){
        if(RAG[dllockid][i] == 1){
            dlpid = i - NLOCK;
        }
    }
    
    //Kill the process involved in the deadlock
    kill(dlpid);

    //Remove the terminated process from any lock wait-queues it may be in
    struct lockentry *lptr;
    for(i=0; i<NLOCK;i++){
        lptr = &locktab[i];
        remove(dlpid, lptr->wait_queue);
    }

    //Unlock the lock's mutex
    mutex_unlock(&locktab[dllockid].lock);

    //Zero out the terminated procs entrires in the RAG
    for(i=0;i<SIZE; i++){
        RAG[dlpid+NLOCK][i] = 0; //Zero out the row for dlpid proc
        RAG[i][dlpid+NLOCK] = 0; //Zero out the column for dlpid proc
    }

    kprintf("DEADLOCK RECOVER   killing pid=%d to free lid=%d\n", dlpid, dllockid);
}