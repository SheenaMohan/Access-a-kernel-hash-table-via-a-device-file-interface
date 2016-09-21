#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "ht530_ioctl.h"

#define PRIO 10
#define NUM_THREADS 4
int set_retval;
int KEY = 20 ;
int DATA = 500;

pthread_mutex_t write_mutex=PTHREAD_MUTEX_INITIALIZER;
int ht_530_read_key(int fd, int key)     				/*call to give input key to the read function*/
{
if((set_retval = ioctl(fd,HT_530_READ_KEY,(unsigned long)&key))!=0){
	printf("ioctl error %d\n", set_retval);}
	
	if(set_retval)
		printf("ht530_dev_error: unable to perform ht_530_read_key \n");
	
	return 0;
}

int dump_ioctl(int fd, int n, ht_obj_t object_array[8])     
{
	int i;
	dump_arg Sdump_arg;
	Sdump_arg.in.in_n = n;
	for(i=0; i<8;i++)
	{Sdump_arg.out.out_object_array[i] = object_array[i];}
	set_retval = ioctl(fd,DUMP_IOCTL,(unsigned long)&Sdump_arg);

	if(set_retval)
		printf("ht530_dev_error: unable to perform dump_ioctl \n");
	
	return Sdump_arg.RetVal;
}

void *test_func(void *fd)
{
	int res,set, i, buf;
	int fptr = *(int*)fd;
	Pht_obj_t new_obj;
	new_obj = (Pht_obj_t) malloc(sizeof(ht_obj_t));
	memset(new_obj, 0, sizeof(ht_obj_t));
	for(i=0; i<50; i++ )
	{
	new_obj->key = KEY++;
	new_obj->data = DATA++;
		pthread_mutex_lock(&write_mutex);
		res = write(fptr, new_obj, sizeof(ht_obj_t));
		if(res<0)
		printf("write operation failed");
		pthread_mutex_unlock(&write_mutex);
	sleep(1);
	set = ht_530_read_key( fptr,  new_obj->key);      /*call to give input key to the read function*/
	if ((set =read( fptr, &buf , sizeof(int)))==-1)     
	printf("no data is read\n");
	else
	printf("data read from the hash table =  %d\n ", buf);
	}

	

	pthread_exit(0);
}


int main(int argc, char **argv)
{
	int fd, res;
	ht_obj_t object_array[8];
	int i = 0;

	fd = open("/dev/ht530", O_RDWR);
	if (fd < 0 ){
		printf("Can not open device file.\n");		
		return 0;
	}
	else {
		printf("ht530 device is open\n");
	}
	
	pthread_t tid[NUM_THREADS];
	pthread_attr_t attr;
	struct sched_param param[NUM_THREADS];
	pthread_attr_init(&attr);
	
	
	for(i=0; i<NUM_THREADS; i++ )
	{
		param[i].sched_priority = PRIO + i;
		pthread_attr_setschedparam (&attr, &param[i]);
		pthread_create(&tid[i],&attr,test_func,(void*)&fd);
	}
	
	sleep(2);
	for(i=0;i<NUM_THREADS;i++)
		pthread_join(tid[i],NULL);
 

for(i=0; i<128; i++)
res = dump_ioctl( fd, i, object_array);     
if(res<0)
printf("dump_iioctl failed\n");
/* close devices */
		close(fd);
	return 0;
}
