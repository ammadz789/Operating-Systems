#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h> 
#include <queue> 
#include <semaphore.h>
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
	int id;
	int size;
};


queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size
int indicator = 0;
int limit = (MEMORY_SIZE/6);





void my_malloc(int thread_id, int size)
{
	//This function will add the struct to the queue
	node new_node;
	new_node.id = thread_id; new_node.size = size;
	myqueue.push(new_node);
	
}

void * server_function(void *)
{
	//This function should grant or decline a thread depending on memory size.
	int address = 0;
	while (indicator == 0)
	{
		pthread_mutex_lock(&sharedLock);
		
		if (!myqueue.empty())
		{
			node toCheck = myqueue.front();
			myqueue.pop();		
		
			if (address + toCheck.size <= MEMORY_SIZE)
			{
				thread_message[toCheck.id] = address;
				address += toCheck.size;
			}
			else
			{
				thread_message[toCheck.id] = -1;
			}
			sem_post(&semlist[toCheck.id]);
		}
		pthread_mutex_unlock(&sharedLock);		
	}

	pthread_exit(NULL);
}


void * thread_function(void * id) 
{
	//This function will create a random size, and call my_malloc
	int randMS = (rand() % limit + 1); // random number for memory size of thread between memorysize/6 and 1
	int *tID = (int *) id;
	pthread_mutex_lock(&sharedLock);
	my_malloc(*tID, randMS);
	pthread_mutex_unlock(&sharedLock);
	

	//Block
	sem_wait(&semlist[*tID]);
	int count = 0;


	//Then fill the memory with id's or give an error prompt
	if (thread_message[*tID] != -1)
	{
		while (count < randMS)
		{
			memory[thread_message[*tID] + count] = ('0' + *tID);
			count++;
		}
	}
	else
	{
		cout<<"Memory is not available for thread: "<<*tID<<endl;
	}
	
}

void init()	 
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{sem_init(&semlist[i],0,0);}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory 
  	{char zero = '0'; memory[i] = zero;}
   	pthread_create(&server,NULL,server_function,NULL); //start server 
	pthread_mutex_unlock(&sharedLock); //unlock
}



void dump_memory() 
{
 // You need to print the whole memory array here.
	cout<<"-------------------------------------------------------------------------------"<<endl;
	for (int i = 0; i < MEMORY_SIZE; i++)
	{
		cout<<memory[i];
	}
}

int main (int argc, char *argv[])
 {

 	//You need to create a thread ID array here
	pthread_t threads[NUM_THREADS];
	int threadID[NUM_THREADS];	 

 	init();	// call init

 	//You need to create threads with using thread ID array, using pthread_create()
	srand(time(NULL));                            //for obtaining random integer for memorysize
	for (int i = 0; i < NUM_THREADS; i++)
	{
		threadID[i] = i;
		pthread_create(&threads[i], NULL, thread_function, &threadID[i]);
	}

 	//You need to join the threads
	for (int i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(threads[i],NULL);
	}
	indicator = 1;
	pthread_join (server, NULL);
	
	
 	dump_memory(); // this will print out the memory
	
 	
 	printf("\nMemory Indexes:\n" );
 	for (int i = 0; i < NUM_THREADS; i++)
 	{
 		printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
 	}
 	printf("\nTerminating...\n");
 }