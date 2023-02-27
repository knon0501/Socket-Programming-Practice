#include <stdio.h>
#include <pthread.h>
#include <dispatch/dispatch.h>


dispatch_semaphore_t sem_one;
dispatch_semaphore_t sem_two;
void * r(void * arg);
void * accu(void * arg);

static int num;

int main(int argc, char *argv[])
{
	pthread_t id_t1, id_t2;

    sem_one=dispatch_semaphore_create(0);
    sem_two=dispatch_semaphore_create(1);
	pthread_create(&id_t1, NULL, r, NULL);
	pthread_create(&id_t2, NULL, accu, NULL);

	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);

    dispatch_release(sem_one);
    dispatch_release(sem_two);
	return 0;
}

void * r(void * arg)
{
	int i;
	for(i=0; i<5; i++)
	{
		fputs("Input num: ", stdout);
		dispatch_semaphore_wait(sem_two,DISPATCH_TIME_FOREVER);
		scanf("%d", &num);
		dispatch_semaphore_signal(sem_one);
	}
	return NULL;	
}
void * accu(void * arg)
{
	int sum=0, i;
	for(i=0; i<5; i++)
	{
		dispatch_semaphore_wait(sem_one,DISPATCH_TIME_FOREVER);
		sum+=num;
		dispatch_semaphore_signal(sem_two);
	}
	printf("Result: %d \n", sum);
	return NULL;
}