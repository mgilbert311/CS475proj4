//Base 0 so 1 less
//#define NLOCK 9
//#define NPROC 19
#define NLOCK 10
#define NPROC 20
#define SIZE NLOCK + NPROC
//#define *matrix //The adjacency matrix
//For cycle detection
#define WHITE 0
#define GREY 1
#define BLACK 2
#define LOCK 0
#define THREAD 1


void rag_request(int pid, int lockid);
void rag_alloc(int pid, int lockid);
void rag_dealloc(int pid, int lockid);
void rag_print();
int translateIndex(int v);
void deadlock_detect(void);
int deadlock_helper(int index);
void print_parent(int index);

