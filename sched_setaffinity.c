#include <stdio.h>
#include <math.h>   // -lm
#include <sched.h>
#include <pthread.h> //lpthread
 
double waste_time(long n)
{
    double res = 0;
    long double i = 0;
    for(i=0; i <n * 200000; i++)
        res += sqrt(i);

    return res;
}

void  *work1(void *arg)
{
    unsigned long mask = 1; /* processor 0 */
 
    /* bind process to processor 0 */
    if (sched_setaffinity(0, sizeof(mask), &mask) <0) { // int sched_setaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask);
        perror("sched_setaffinity error\n");
    }
 
    /* waste some time so the work is visible with "top" */
    printf ("result: %f\n", waste_time (20000));


    return 0;
}
 

void *work2(void *arg)
{

    unsigned long mask = 2; /* process switches to processor 1 now */

    if (sched_setaffinity(0, sizeof(mask), &mask) <0) { 
        perror("sched_setaffinity error\n");
    }
 
    /* waste some more time to see the processor switch */
    printf ("result: %f\n", waste_time (20000));

    return 0;
}


/*
    int
    pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void *), void *restrict arg);

    分别绑定不同CPU核心
*/


int main(int argc, char **argv)
{
    int err = 0;
    pthread_t t_id1, t_id2;

    if( pthread_create (&t_id1, NULL, &work1, NULL) != 0)
         printf("Error err = %d\n", err);
    

    if(pthread_create (&t_id2, NULL, &work2, NULL) != 0)
        printf("Error err = %d\n", err);

    pthread_join(t_id1, NULL);
    pthread_join(t_id2, NULL);
    
    return 0;
}
