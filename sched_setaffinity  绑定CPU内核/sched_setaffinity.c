#include <math.h>   // -lm
#include <sched.h>
 
double waste_time(long n)
{
    double res = 0;
    long double i = 0;
    for(i=0; i <n * 200000; i++)
　　　　 res += sqrt(i);
    return res;
}
 
int main(int argc, char **argv)
{
    unsigned long mask = 1; /* processor 0 */
 
    /* bind process to processor 0 */
    if (sched_setaffinity(0, sizeof(mask), &mask) <0) { // int sched_setaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask);
        perror("sched_setaffinity error\n");
    }
 
    /* waste some time so the work is visible with "top" */
    printf ("result: %f\n", waste_time (200000));
 
    mask = 2; /* process switches to processor 1 now */
    if (sched_setaffinity(0, sizeof(mask), &mask) <0) { 
        perror("sched_setaffinity error\n");
    }
 
    /* waste some more time to see the processor switch */
    printf ("result: %f\n", waste_time (20000));
}