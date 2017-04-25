/**
* Reads in the file and builds the RAG then checks for deadlock
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RAG.h"

int main(int argc, char** argv){
    FILE *fp;
    char fileBuffer[50];
    char const t[2] = ","; //The delimiter
    int pid, lockid;
    char *token;

    //Open the file
    fp = fopen(argv[1], "rt");
    if(fp != NULL){
        while(fgets(fileBuffer, 10, fp) != NULL){
            token = strtok(fileBuffer, t);
            pid = atoi(token);
            token = strtok(NULL, t);
            if(strcmp(token,"R") == 0){
                token = strtok(NULL, t);
                lockid = atoi(token); 
                rag_request(pid, lockid);
            }else if(strcmp(token, "A")==0){
                //Must be an allocation 

                token = strtok(NULL, t);
                lockid = atoi(token); 
                rag_alloc(pid, lockid);
            }else if(strcmp(token, "D")==0){
                token = strtok(NULL, t);
                lockid = atoi(token); 

                rag_dealloc(pid, lockid);
            }else{
                printf("That is not a valid code\n");
                exit(1);
            }
        }

        fclose(fp);
    }
    else{
        printf("Error problem opening file\n");
        exit(1);
    }
    //Run the deadlock detection
    deadlock_detect();
    rag_print();
    return 0;
}