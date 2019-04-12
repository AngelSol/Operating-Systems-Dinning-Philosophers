 //Angel Solis
//CS 370

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


typedef struct philosopher {	
	sem_t *room;		// pointer to the shared room semaphore
	sem_t *left,*right; // for the forks
	int index,type;
	
} philosopher;

void thread_algorithm(philosopher* phil)
{
	int i;
	for(i=0;i<5;i++)
	{
		printf("Philosopher %d: is done thinking and now ready to eat.\n",phil->index);
	
		if(phil->type ==0) // check deadlock condition 
			sem_wait(phil->room);
			
	
		sem_wait(phil->left); // grab left fork then give out message
		printf("Philosopher %d: taking left fork %d\n",phil->index,phil->index);
		
		usleep(10); // sleep for 10 usec to help deadlock occour
		
	
		sem_wait(phil->right); // grab right fork and give out message
		printf("Philosopher %d: taking right fork %d\n",phil->index,phil->index+1);
		
		
		printf("Philosopher %d: EATING.\n",phil->index);
		
	
		printf("Philosopher %d is putting down left fork %d\n",phil->index,phil->index);
		sem_post(phil->left); // print message then put down left fork
		
		
		printf("Philosopher %d is putting down right fork %d\n",phil->index,phil->index+1);
		sem_post(phil->right); // print message then put down right fork
		
		
		if(phil->type ==0)	 // check deadlock condition
			sem_post(phil->room);
	}
	printf("Philosopher %d: is done eating.\n",phil->index);
}

int main(int argc, char *argv[])
{
	int type = *argv[1]-'0'; // convert ascii character to an integer 
	int i; // used in loops
	philosopher* phillies=calloc (5,sizeof(philosopher)); 
	sem_t forks[5];			//fork sem array
	for (i=0;i<5;i++)		// start initializing the semaphores
		sem_init(&forks[i],0,1);
	sem_t room;			// create & initialize the room semaphore 
	sem_init(&room,0,4);	// only allow four philosophers in
	
	for(i=0;i<5;i++) //initalize phils
		{
			phillies[i].left = &forks[i];
			phillies[i].room = &room;
			phillies[i].type=type;
			phillies[i].index = i;
			phillies[i].right = &forks[(i+1)%5];

		}
	
	
	pthread_t threadids[5];
	
	for (i=0;i<5;i++)
	{
		pthread_create(&threadids[i],NULL, (void *) thread_algorithm,(void*)&phillies[i]);
	}

	for (i=0;i<5;i++)
	{
		pthread_join(threadids[i],NULL);
	}
	free(phillies);
	return 0;
}
